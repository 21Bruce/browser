#include "../../lib/large.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    struct bksmt_bigint *r1, *r2, *r3;

    uint64_t n1[] = { 0 }; 
    uint64_t n2[] = { 0 }; 
    uint64_t *n3;
    uint64_t one;
    size_t size;
    int sign;

    /* 0xC0000000
    one = 0x1;
    n1[0] = (one << 63) + (one << 62);
    n2[0] = (one << 63) + (one << 62);
    r1 = bksmt_bigint_init_lst(n1, 1, 0);
    r2 = bksmt_bigint_init_lst(n2, 1, 0);
    r3 = bksmt_bigint_add(r1, r2);

    bksmt_bigint_lst_rep(r3, &n3, &size, &sign); 

    if (sign != 0)
        return 1;

    if (size != 2)
        return 2;

    if (n3[0] != 0x8000000000000000 || n3[1] != 1)
        return 3;

    free(n3);

    n2[0] += one + one;
    r1 = bksmt_bigint_init_lst(n1, 1, 0);
    r2 = bksmt_bigint_init_lst(n2, 1, 1);
    r3 = bksmt_bigint_add(r1, r2);
    bksmt_bigint_lst_rep(r3, &n3, &size, &sign); 

    if (size != 1)
        return 4;

    if (n3[0] != one + one)
        return 5;

    r3 = bksmt_bigint_add(r2, r1);
    bksmt_bigint_lst_rep(r3, &n3, &size, &sign); 

    fprintf(stderr, "%d\n", r1->size);
    fprintf(stderr, "%d\n", r1->sign);
    fprintf(stderr, "%llu\n", r1->num[0]);

    fprintf(stderr, "\n%llu\n", n3[0]);
    fprintf(stderr, "%llu\n", n3[1]);


    if (n3[0] != one + one)
        return 7;

    if (size != 1)
        return 6;


    return 0;
}
