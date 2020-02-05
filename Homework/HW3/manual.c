#include "manual.h"

#include <stdio.h>

void manual()
{
    printf("\nSUPPORTED COMMANDS AND OPERATIONS\n\n");

    printf("\033[1m\033[37m./bk8000l_conf [binary file] -s [parameter] [value] \033[0m\nValue of [parameter] in configuration file [binary file] is changed to [value]. The correspoding parameter bit is turned on.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -S [parameter] [value] \033[0m\nWorks like -s, however the parameter bit is not changed.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -g [parameter] \033[0m\nReturns value of [parameter] only if activated.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -G [parameter] \033[0m\nReturns value of [parameter], whenever it is activated.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -l \033[0m\nReturns all parameters the device uses.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -L \033[0m\nReturns all parameters (regardless whether active or not).\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -l [parameter1] [parameter2]...[parameterN] \033[0m\nReturns values of given parameter(s) the device uses.\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -L [parameter1] [parameter2]...[paramaterN] \033[0m\nReturns values of given parameter(s) the device uses (regardless whether active or not).\n\n");
    printf("\033[1m\033[37m./bk8000l_conf [binary file] -b [parameter] [0/1] \033[0m\n(De)activates parameter bit based on given value (zero or one)\n\n");
    printf("\033[1m\033[37m./bk8000l_conf new.bin -c [segNo1] [segType1] [segNo2] [segType2]...[segNoN] [segTypeN] \033[0m\nCreates empty configuration file based on given parameters. Permitted segment types: t - text; n(numeric); b(byte).\n\n");
    printf("\033[1m\033[37m./bk8000l_conf -h - manual\033[0m\n\n");
}
