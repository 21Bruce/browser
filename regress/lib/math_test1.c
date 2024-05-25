#include "../../lib/math.h"

#include <stdio.h>

int main(void)
{
    if (intpow(2,2) != 4) {
        fprintf(stderr, "test intpow(2,2) == 4 FAILED\n");
        return 1;
    }

    if (intpow(2,3) != 8) {
        fprintf(stderr, "test intpow(2,3) == 8 FAILED\n");
        return 2;
    }

    if (intpow(3,2) != 9) {
        fprintf(stderr, "test intpow(3,2) == 9 FAILED\n");
        return 3;
    }

    if (intpow(2,0) != 1) {
        fprintf(stderr, "test intpow(2,0) == 1 FAILED\n");
        return 4;
    }

    if (intpow(0,2) != 0) {
        fprintf(stderr, "test intpow(0,2) == 0 FAILED\n");
        return 5;
    }

    if (intpow(-2,2) != 4) {
        fprintf(stderr, "test intpow(-2,2) == 4 FAILED\n");
        return 6;
    }

    if (intpow(-2,3) != -8) {
        fprintf(stderr, "test intpow(-2,3) == -8 FAILED\n");
        return 7;
    }

    if (roundup(2,3) != 3) {
        fprintf(stderr, "test roundup(2,3) == 3 FAILED\n");
        return 8;
    }

    if (roundup(3,2) != 4) {
        fprintf(stderr, "test roundup(3,2) == 4 FAILED\n");
        return 9;
    }

    if (roundup(5,2) != 6) {
        fprintf(stderr, "test roundup(5,2) == 6 FAILED\n");
        return 10;
    }

    if (roundup(5,4) != 8) {
        fprintf(stderr, "test roundup(5,4) == 8 FAILED\n");
        return 11;
    }

    if (rounddown(2,3) != 0) {
        fprintf(stderr, "test rounddown(2,3) == 0 FAILED\n");
        return 12;
    }

    if (rounddown(3,2) != 2) {
        fprintf(stderr, "test rounddown(3,2) == 2 FAILED\n");
        return 13;
    }

    if (rounddown(5,2) != 4) {
        fprintf(stderr, "test rounddown(5,2) == 4 FAILED\n");
        return 14;
    }

    if (rounddown(9,4) != 8) {
        fprintf(stderr, "test rounddown(9,4) == 8 FAILED\n");
        return 15;
    }

    if (pmod(9,4) != 1) {
        fprintf(stderr, "test pmod(9,4) == 1 FAILED\n");
        return 16;
    }

    if (pmod(-9,4) != 3) {
        fprintf(stderr, "test pmod(-9,4) == 3 FAILED\n");
        return 17;
    }

    fprintf(stderr, "all tests succeeded\n");
    return 0;
}
