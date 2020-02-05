#!/bin/bash
# Homework 2
# Momchil Ignatov

RED='\033[0;31m'
ORANGE='\033[0;33m'
NC='\033[0m' # No Color

mkdir -p $4 2>/dev/null

if [[ $? -ne 0 ]]; then
  echo -e "${RED}Cannot create output directory."
  exit 1
fi

# Check if output directory is accessible
if [[ ! -w $4 || ! -r $4 || ! -x $4 ]]; then
  echo -e "${RED}Output directory permission denied."
  exit 1
fi

# Check if input files exist
for arg in $@; do
  if [[ ! -e ${arg} ]]; then
    echo -e "${RED}'${arg}' does not exist."
    exit 1
  fi
done

declare -r AUDIO_START=$(grep '^beep' $1 | cut -f3 -d' ')

if [[ -z ${AUDIO_START} ]]; then
  echo -e "${RED}Mandatory record 'beep' not found in ${1}"
  exit 1
fi

declare -r SAMPLING_RATE=$(grep 'SamplingRate' $2 \
  | sed -r 's/<\/*[a-zA-Z]*>//g' \
  | grep -Eo '^[0-9]*')

if [[ -z ${SAMPLING_RATE} ]]; then
  echo -e "${RED}Mandatory record 'SamplingRate' not found in ${2}"
  exit 1
fi

declare -r START_RECORDING_DATE=$(grep 'StartRecordingDate' $2 \
  | sed -r 's/<\/*[a-zA-Z]*>//g' \
  | sed -r 's/([0-9]*).([0-9]*).([0-9]*)/\3\/\2\/\1/g')

if [[ -z ${START_RECORDING_DATE} ]]; then
  echo -e "${RED}Mandatory record 'StartRecordingDate' not found in ${2}"
  exit 1
fi

declare -r START_RECORDING_TIME=$(grep 'StartRecordingTime' $2 | sed -r 's/<\/*[a-zA-Z]*>//g')

if [[ -z ${START_RECORDING_TIME} ]]; then
  echo -e "${RED}Mandatory record 'StartRecordingTime' not found in ${2}"
  exit 1
fi

declare -r RECORD_LENGTH=$(grep 'RecordLength' $2 \
  | sed -r 's/<\/*[a-zA-Z]*>//g' \
  | awk -F: '{ print ($1 * 3600) + ($2 * 60) + $3 }')

if [[ -z ${RECORD_LENGTH} ]]; then
  echo -e "${RED}Mandatory record 'RecordLength' not found in ${2}"
  exit 1
fi

declare -r DATE=$(TZ=${EEG_TZ} date -d "${START_RECORDING_DATE} ${START_RECORDING_TIME}" +%s)

declare -r FIRST_TICK=$(awk '/<tick>/{ print NR; exit }' $2)

declare -r AUDIO_LENGTH=$(sox $3 -n stat 2>&1 \
  | sed -n 's#^Length (seconds):[^0-9]*\([0-9.]*\)$#\1#p')

# Check if duplicate records exist
if [[ -n $(sort $1 -r | uniq -d) ]]; then
  echo -e "${RED}Duplicate records found."
  exit 1
fi

while IFS='' read -r record || [[ -n "$record" ]]; do

  stimulus=$(echo "$record" | sed -r 's/^(.*) (.*) (.*)$/\1/g')
  start_stimulus=$(echo "$record" | sed -r 's/^(.*) (.*) (.*)$/\2/g')
  end_stimulus=$(echo "$record" | sed -r 's/^(.*) (.*) (.*)$/\3/g')

  # Check if record is out of bounds
  if [[ $(echo "${DATE} > ${start_stimulus}" | bc -l) = 1 \
    || $(echo "${DATE} + ${RECORD_LENGTH} < ${end_stimulus}" | bc -l) = 1 \
    || $(echo "${AUDIO_START} > ${start_stimulus}" | bc -l) = 1 \
    || $(echo "${AUDIO_START} + ${AUDIO_LENGTH} < ${end_stimulus}" | bc -l) = 1 ]]; then
      echo -e "${ORANGE}Warning! Skipping ${stimulus}. Out of bounds.${NC}"
      continue
  fi

  # WAV Generation
  start_audio=$(bc -l <<< "${start_stimulus}-${AUDIO_START}")
  duration=$(bc -l <<< "${end_stimulus}-${start_stimulus}")

  if [[ $(echo "${duration} < 0.2" | bc -l) = 1 ]]; then
    echo -e "${ORANGE}Warning! Skipping ${stimulus}. Duration is less than 0.2 sec.${NC}"
    continue
  fi

  sox $3 $4/${stimulus}_lar.wav trim ${start_audio} ${duration}

  # XML Generation
  start_row=$(printf "%.0f" $(bc -l <<< "(${start_stimulus}-${DATE})*${SAMPLING_RATE}-${FIRST_TICK}"))
  records=$(printf "%.0f" $(bc -l <<< "(${end_stimulus}-${start_stimulus})*${SAMPLING_RATE}"))
  cat $2 | tail -n +${start_row} | head -n ${records} > $4/${stimulus}_eeg.xml

done < <(grep -v "^beep" $1)
