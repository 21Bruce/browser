#include "strconv.h"
#include "xstring.h"
#include "xmalloc.h"
#include "math.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

char *
inttocstr(int src)
{
    int tmp, nlen;
    char *ret;

    for(tmp = src, nlen = 1; tmp >= 10; tmp /= 10, nlen++) 
        ;

    ret = xmallocarray(nlen + 1, sizeof *ret);
    ret[nlen] = 0;
    for(--nlen; nlen >= 0; nlen--, src /= 10)
        ret[nlen] = '0' + (src % 10);
    return ret;
}

int
cstrtoint(char *str)
{
    int i, sum;
    char c;

    i = strlen(str) - 1;
    sum = 0;
    while((c = *str++)) {
        sum += (c - '0') * intpow(10,i);
        i--;
    }
    return sum;
}

int
csubstrtoint(char *start, char *end)
{
    assert(end > start);

    int i, sum;
    char c;

    i = end - start - 1;
    sum = 0;
    while(start != end && (c = *start++)) {
        sum += (c - '0') * intpow(10,i);
        i--;
    }
    return sum;
}
