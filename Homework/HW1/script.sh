sort -u dic.txt > dic_filter.txt;

grep -o '[[:alpha:]]\+' text.txt |
tr '[:upper:]' '[:lower:]' |
sort -u > text_list.txt;

grep -v -x -F -f dic_filter.txt text_list.txt > text_filter.txt;

while read word; do 
 printf '%s:' $word;
 tre-agrep -s -B -w $word dic_filter.txt; 
done <text_filter.txt |
grep '^[a-z].*';

rm dic_filter.txt text_list.txt text_filter.txt;