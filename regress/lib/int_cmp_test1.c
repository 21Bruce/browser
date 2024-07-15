#include "../../lib/int.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


int
main(void)
{
    struct bksmt_int *i1, *i2;

    uint64_t num1[2] = {1, 1}, num2[2] = {0, 1};
    uint64_t num11[2] = {1, 1}, num22[2] = {0, 1};
    uint64_t num111[2] = {1, 1}, num222[2] = {1, 1};
    uint64_t num1111[2] = {1, 0, 1}, num2222[2] = {1, 1};

    i1 = bksmt_int_init_lst(num1, 2, 1);
    i2 = bksmt_int_init_lst(num2, 2, 1);

    if (bksmt_int_cmp(i1, i2) != 1) 
        return 1;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    i1 = bksmt_int_init_lst(num11, 2, -1);
    i2 = bksmt_int_init_lst(num22, 2, 1);

    if (bksmt_int_cmp(i1, i2) != -1) 
        return 2;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    i1 = bksmt_int_init_lst(num111, 2, 1);
    i2 = bksmt_int_init_lst(num222, 2, 1);

    if (bksmt_int_cmp(i1, i2) != 0) 
        return 3;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    i1 = bksmt_int_init_lst(num1111, 3, 1);
    i2 = bksmt_int_init_lst(num2222, 2, 1);

    if (bksmt_int_cmp(i1, i2) != 1) 
        return 4;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    i1 = bksmt_int_init_lst(num1111, 3, -1);
    i2 = bksmt_int_init_lst(num2222, 2, -1);

    if (bksmt_int_cmp(i1, i2) != -1) 
        return 5;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    i2 = bksmt_int_init_lst(num1111, 3, 1);
    i1 = bksmt_int_init_lst(num2222, 2, 1);

    if (bksmt_int_cmp(i1, i2) != -1) 
        return 6;

    bksmt_int_free(i1);
    bksmt_int_free(i2);

    return 0;

}
