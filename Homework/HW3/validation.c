#include "validation.h"

#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int getNumber(char c)
{
	switch (c)
	{
	case 't':
		return 0;
	case 'n':
		return 1;
	case 'b':
		return 2;
	default:
		errx(1, "Invalid input");
	}
}

void byteValidation(const struct base *parameter, char *value)
{
	const char separators[3] = " ,";

	char validNumbers[MAXN];
	strcpy(validNumbers, parameter->validation);

	char *token = strtok(validNumbers, separators);

	while (token != NULL)
	{
		if (!strcmp(token, value))
		{
			return;
		}
		token = strtok(NULL, separators);
	}
	errx(1, "Validation failed");
}

char *regexify(char regx[], char *value)
{
	regx[0] = '^';
	regx[1] = '\0';
	strcat(regx, value);
	strcat(regx, "+$");
	return regx;
}

void textValidation(const struct base *parameter, char *value)
{
	char regx[MAXN];
	regexify(regx, parameter->validation);
	regex_t regex;
	int reti = regcomp(&regex, regx, REG_EXTENDED);
	if (reti)
	{
		errx(1, "Could not compile regex");
	}

	reti = regexec(&regex, value, 0, NULL, 0);
	if (reti)
	{
		errx(1, "Validation failed");
	}

	regfree(&regex);
}

void validate(const struct base *parameter, char *value)
{
	const int type = SEG_ORDER[parameter->segment];

	switch (type)
	{
	case TEXT_SIZE:
		textValidation(parameter, value);
		break;
	case NUM_SIZE:
	case BYTE_SIZE:
		byteValidation(parameter, value);
		break;
	default:
		errx(1, "Validation failed");
	}
}

void validation(int argc, char *argv[])
{
	if (argc == 2)
	{
		if (strlen(argv[1]) != 2 || argv[1][1] != 'h')
		{
			errx(2, "Invalid option format");
		}
	}
	else if (argc >= 3)
	{
		if (strlen(argv[2]) != 2 || argv[2][0] != '-')
		{
			errx(2, "Invalid option format");
		}
		else if (argc == 3 && !((argv[2][1] == 'l') || (argv[2][1] == 'L')))
		{
			errx(2, "Invalid option format");
		}
	}
	else
	{
		errx(2, "Invalid number of arguments");
	}
}