#include "math.h"

int
intpow(int b, int e)
{
    int i = 0, exp = 1;

    while (i < e) {
        exp *= b;
        i++;
    }

    return exp;
}

int
roundup(int src, int mul) 
{
    if (src % mul != 0) 
       return  (src/mul + 1) * mul;

    return src;
}

int
rounddown(int src, int mul) 
{
    return (src/mul) * mul;
}

long
pmod(long src, long base)
{
    long ret;

    ret = src % base;
    while(ret < 0)
        ret += base;

    return ret;
}
