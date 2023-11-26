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
