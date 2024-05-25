#include "../../lib/math.h"

int main(void)
{
    if (intpow(2,2) != 4)
        return 1;

    if (intpow(2,3) != 8)
        return 2;

    if (intpow(3,2) != 9)
        return 3;

    if (intpow(2,0) != 1)
        return 4;

    if (intpow(0,2) != 0)
        return 5;

    if (intpow(-2,2) != 4)
        return 6;

    if (intpow(-2,3) != -8)
        return 7;

    if (roundup(2,3) != 3)
        return 8;

    if (roundup(3,2) != 4)
        return 9;

    if (roundup(5,2) != 6)
        return 10;

    if (roundup(5,4) != 8)
        return 11;

    if (rounddown(2,3) != 0)
        return 12;

    if (rounddown(3,2) != 2)
        return 13;

    if (rounddown(5,2) != 4)
        return 14;

    if (rounddown(9,4) != 8)
        return 15;

    if (pmod(9,4) != 1)
        return 16;

    if (pmod(-9,4) != 3)
        return 17;
    return 0;
}
