#include "../../lib/large.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int
main(void)
{
    struct bksmt_bigint *r1, *r2;

    uint64_t n1[] = { 0 }; 
    uint64_t n2[] = { 0 }; 
    uint64_t *n3;
    uint64_t one;
    size_t size;
    int sign;

    one = 0x1;
    n1[0] = one << 63;
    n2[0] = one << 63;
    r1 = bksmt_bigint_init_lst(n1, 1, 0);
    r2 = bksmt_bigint_init_lst(n2, 1, 0);
    bksmt_bigint_adds(r1, r2);

    bksmt_bigint_lst_rep(r1, &n3, &size, &sign); 

    if (sign != 0)
        return 1;

    if (size != 2)
        return 2;

    if (n3[0] != 0 || n3[1] != 1)
        return 3;

    return 0;
}
