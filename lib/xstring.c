#include "xstring.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <err.h>

#define ENOMEM -1

int
xasprintf(char **ret, const char * restrict format, ...)
{
    va_list ap;
    int stat;

    va_start(ap, format);
    stat = vasprintf(ret, format, ap);
    va_end(ap);

    if (stat == -1) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    } 

    return stat;
}


