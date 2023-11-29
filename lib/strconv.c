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

int
hexcstrtoint(char *str)
{
    int i, sum, num;
    char c;

    i = strlen(str) - 1;
    sum = 0;
    while((c = *str++)) {
        if ('0' <= c && c <= '9')
            num = c - '0';
        else 
            switch(c){
                case 'A':
                case 'a':
                    num = 10;
                    break;
                case 'B':
                case 'b':
                    num = 11;
                    break;
                case 'C':
                case 'c':
                    num = 12;
                    break;
                case 'D':
                case 'd':
                    num = 13;
                    break;
                case 'E':
                case 'e':
                    num = 14;
                    break;
                case 'F':
                case 'f':
                    num = 15;
                    break;
            }
        sum += num * intpow(16,i);
        i--;
    }
    return sum;
}

int
hexcsubstrtoint(char *start, char *end)
{
    assert(end > start);

    int i, sum, num;
    char c;

    i = end - start - 1;
    sum = 0;
    while(start != end && (c = *start++)) {
        if ('0' <= c && c <= '9')
            num = c - '0';
        else 
            switch(c){
                case 'A':
                case 'a':
                    num = 10;
                    break;
                case 'B':
                case 'b':
                    num = 11;
                    break;
                case 'C':
                case 'c':
                    num = 12;
                    break;
                case 'D':
                case 'd':
                    num = 13;
                    break;
                case 'E':
                case 'e':
                    num = 14;
                    break;
                case 'F':
                case 'f':
                    num = 15;
                    break;
            }
        sum += num * intpow(16,i);
        i--;
    }
    return sum;
}
