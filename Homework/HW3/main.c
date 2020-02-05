#include <stdio.h>
#include <err.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "commons.h"
#include "validation.h"
#include "manual.h"

char fileName[MAXN];

int openFile(const char *FILE_NAME, const int FLAG)
{
	int fd = open(FILE_NAME, FLAG, 00744);
	if (fd == -1)
	{
		errx(1, "File failed to open\n");
	}
	return fd;
}

void createFile(const char *name, char *argv[], int start, int end)
{
	int fd = openFile(name, O_CREAT | O_TRUNC | O_RDWR);
	lseek(fd, 0, SEEK_SET);

	if ((end - start) % 2 != 0)
	{
		errx(1, "Invalid input");
	}

	for (int i = 0; i < ((end - start) / 2) * SEGMENT_SIZE; i++)
	{
		write(fd, "\x00", 1);
	}

	unsigned char pos[2] = "0";

	for (size_t i = start; i < end; i += 2)
	{
		if (strcmp(pos, argv[i]))
		{
			errx(1, "Invalid input");
		}

		int position = SEGMENT_SIZE * (pos[0] - '0');
		char number[2];
		number[0] = getNumber(argv[i + 1][0]);

		lseek(fd, position, SEEK_SET);
		write(fd, number, 1);
		pos[0]++;
	}

	close(fd);
}

void changeBit(const struct base *parameter, const char *bit)
{
	int fd = openFile(fileName, O_RDWR);

	const int DATA_SIZE = 7;
	unsigned char data[DATA_SIZE];

	const int START_POSITION = 1 + (parameter->segment * SEGMENT_SIZE);

	lseek(fd, START_POSITION, SEEK_SET);

	if (read(fd, &data, sizeof(data)) != sizeof(data))
	{
		close(fd);
		errx(1, "Error while reading");
	}

	int group = parameter->position / 8;
	int shift = DATA_SIZE - (parameter->position - (8 * group));
	char mask = (1 << shift);

	data[group] &= ~mask;
	if (!strcmp(bit, "1"))
	{
		data[group] |= mask;
	}

	lseek(fd, START_POSITION, SEEK_SET);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		if (write(fd, &data[i], sizeof(data[i])) != sizeof(data[i]))
		{
			close(fd);
			errx(1, "Error while writing");
		}
	}

	close(fd);
}

const struct base *findParameterByName(char *paramName)
{
	const struct base *parameter;
	int flag = 0;

	for (size_t i = 0; i < parametersSize; i++)
	{
		const char *currentParamName = parameters[i].parameter;
		if (!strcmp(currentParamName, paramName))
		{
			parameter = &parameters[i];
			flag = 1;
			break;
		}
	}

	if (!flag)
	{
		errx(2, "No such parameter\n");
	}

	return parameter;
}

const struct base *setParameterValue(const struct base *parameter, char *value)
{
	validate(parameter, value);

	int fd = openFile(fileName, O_RDWR);

	int segmentNumber = parameter->segment;
	const int bytes = SEG_ORDER[segmentNumber];
	int position = parameter->position;
	int writePos = 8 + (segmentNumber * SEGMENT_SIZE + position * bytes);

	char valueArr[bytes];
	strcpy(valueArr, value);

	for (size_t i = strlen(value); i < bytes; i++)
	{
		valueArr[i] = '\x00';
	}

	lseek(fd, writePos, SEEK_SET);

	if (write(fd, &valueArr, strlen(valueArr)) != strlen(valueArr))
	{
		close(fd);
		errx(1, "Error while writing");
	}

	close(fd);

	return parameter;
}

void getParameterValue(const struct base *parameter)
{
	int fd = openFile(fileName, O_RDONLY);

	int segmentNumber = parameter->segment;
	const int bytes = SEG_ORDER[segmentNumber];
	int writePos = 8 + (segmentNumber * SEGMENT_SIZE + parameter->position * bytes);

	lseek(fd, writePos, SEEK_SET);

	char data[bytes];

	if (read(fd, &data, sizeof(data)) != sizeof(data))
	{
		close(fd);
		errx(1, "Error while reading");
	}
	printf("%s - %s\n", parameter->parameter, data);

	close(fd);
}

bool isParameterEnabled(const struct base *parameter)
{
	int fd = openFile(fileName, O_RDONLY);

	const int DATA_SIZE = 7;
	unsigned char data[DATA_SIZE];

	const int START_POSITION = 1 + (parameter->segment * SEGMENT_SIZE);
	lseek(fd, START_POSITION, SEEK_SET);

	if (read(fd, &data, sizeof(data)) != sizeof(data))
	{
		close(fd);
		errx(1, "Error while reading");
	}
	close(fd);

	int group = parameter->position / 8;
	int shift = DATA_SIZE - (parameter->position - (8 * group));
	char mask = (1 << shift);

	if (data[group] & mask)
	{
		return true;
	}
	return false;
}

void getEnabledParameters()
{
	for (size_t i = 0; i < parametersSize; i++)
	{
		if (isParameterEnabled(&parameters[i]))
		{
			getParameterValue(&parameters[i]);
		}
	}
}

void getEnabledGivenParameters(int argc, char *argv[])
{
	for (size_t i = 3; i < argc; i++)
	{
		const struct base *parameter = findParameterByName(argv[i]);
		if (isParameterEnabled(parameter))
		{
			getParameterValue(parameter);
		}
	}
}

void getAllParameters()
{
	for (size_t i = 0; i < parametersSize; i++)
	{
		getParameterValue(&parameters[i]);
	}
}

void getAllGivenParameters(int argc, char *argv[])
{
	for (size_t i = 3; i < argc; i++)
	{
		getParameterValue(findParameterByName(argv[i]));
	}
}

void options(int argc, char *argv[])
{
	strcpy(fileName, argv[1]);

	if (argc == 2 && argv[1][1] == 'h')
	{
		manual();
		return;
	}

	switch (argv[2][1])
	{
	case 's':
		changeBit(setParameterValue(findParameterByName(argv[3]), argv[4]), "1");
		break;
	case 'S':
		setParameterValue(findParameterByName(argv[3]), argv[4]);
		break;
	case 'g':
		if (!isParameterEnabled(findParameterByName(argv[3])))
		{
			errx(2, "Parameter not enabled");
		}
		getParameterValue(findParameterByName(argv[3]));
		break;
	case 'G':
		getParameterValue(findParameterByName(argv[3]));
		break;
	case 'l':
		if (argc == 3)
		{
			getEnabledParameters();
		}
		else
		{
			getEnabledGivenParameters(argc, argv);
		}
		break;
	case 'L':
		if (argc == 3)
		{
			getAllParameters();
		}
		else
		{
			getAllGivenParameters(argc, argv);
		}
		break;
	case 'b':
		changeBit(findParameterByName(argv[3]), argv[4]);
		break;
	case 'c':
		createFile(fileName, argv, 3, argc);
		break;
	default:
		errx(2, "Option not available. Check -h for help");
	}
}

int main(int argc, char *argv[])
{
	validation(argc, argv);
	options(argc, argv);
}
