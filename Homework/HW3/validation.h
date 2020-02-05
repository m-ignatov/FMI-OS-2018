#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "commons.h"

int getNumber(char c);

char *regexify(char regx[], char *value);

void textValidation(const struct base *parameter, char *value);

void byteValidation(const struct base *parameter, char *value);

void validate(const struct base *parameter, char *value);

void validation(int argc, char *argv[]);

#endif