#include <stdio.h>
#include <stdlib.h>
#include "../../lib/int.h" 


int
main(void)
{
    struct bksmt_int *i1, *i2, *i3;
    uint64_t *num3;
    size_t size;
    int sign;

    uint64_t num01[2] = {1}; 
    uint64_t num02[2] = {0};
    uint64_t num1[2] = {1, 1}; 
    uint64_t num2[2] = {0, 1};
    uint64_t num11[3] = {0, 1, 1}; 
    uint64_t num22[2] = {0, 1};
    uint64_t num111[1] = {BKSMT_INT_SL_MAX}; 
    uint64_t num222[1] = {1};
    uint64_t num1111[1] = {BKSMT_INT_SL_MAX}; 
    uint64_t num2222[1] = {2};

    i1 = bksmt_int_init_lst(num01, 1, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num02, 1, BKSMT_INT_POS);
    i3 = bksmt_int_mul(i1, i2);

    bksmt_int_lst_rep(i3, &num3, &size, &sign);

    bksmt_int_free(i3);
    bksmt_int_free(i2);
    bksmt_int_free(i1);

    if (sign != BKSMT_INT_POS)
        return 1;

    if (size != 1)
        return 2;

    if (num3[0] != 0)
        return 3;

    free(num3);

    i1 = bksmt_int_init_lst(num1, 2, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num2, 2, BKSMT_INT_POS);
    i3 = bksmt_int_mul(i1, i2);

    bksmt_int_lst_rep(i3, &num3, &size, &sign);

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    bksmt_int_free(i3);

    if (sign != BKSMT_INT_POS)
        return 4;

    if (size != 3)
        return 5;

    if (num3[0] != 0 || num3[1] != 1 || num3[2] != 1)
        return 6;

    i1 = bksmt_int_init_lst(num11, 3, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num22, 2, BKSMT_INT_POS);
    i3 = bksmt_int_mul(i1, i2);

    bksmt_int_lst_rep(i3, &num3, &size, &sign);

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    bksmt_int_free(i3);

    if (sign != BKSMT_INT_POS)
        return 7;

    if (size != 4)
        return 8;

    if (num3[0] != 0 || num3[1] != 0 || num3[2] != 1 || num3[3] != 1)
        return 9;

    i1 = bksmt_int_init_lst(num111, 1, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num222, 1, BKSMT_INT_POS);
    i3 = bksmt_int_mul(i1, i2);

    bksmt_int_lst_rep(i3, &num3, &size, &sign);

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    bksmt_int_free(i3);

    if (sign != BKSMT_INT_POS)
        return 10;

    if (size != 1)
        return 11;

    if (num3[0] != BKSMT_INT_SL_MAX)
        return 12;

    i1 = bksmt_int_init_lst(num1111, 1, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num2222, 1, BKSMT_INT_POS);
    i3 = bksmt_int_mul(i1, i2);

    bksmt_int_lst_rep(i3, &num3, &size, &sign);

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    bksmt_int_free(i3);

    if (sign != BKSMT_INT_POS)
        return 13;

    if (size != 2)
        return 14;

    if (num3[0] != BKSMT_INT_SL_MAX - 1 || num3[1] != 1)
        return 15;



    return 0;
 }
