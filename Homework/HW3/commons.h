#ifndef COMMONS_H_
#define COMMONS_H_

#define MAXN 256
#define TEXT_SIZE 16
#define NUM_SIZE 4
#define BYTE_SIZE 1

enum SEG_TYPE
{
    text = TEXT_SIZE,
    numeric = NUM_SIZE,
    byte = BYTE_SIZE
};

struct base
{
    char *parameter;
    int segment;
    int position;
    char *validation;
};

extern const enum SEG_TYPE SEG_ORDER[];
extern const struct base parameters[];
extern int parametersSize;
extern const int SEGMENT_SIZE;

#endif