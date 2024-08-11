#include "../../lib/int.h" 

#include <stdlib.h> 
#include <stdio.h> 


int
main(void)
{
    uint64_t *num3;
    struct bksmt_int *i1, *i2;
    size_t size;
    int sign;

    uint64_t num1[2] = {0, 2};
    uint64_t num2[2] = {1, 1}; 
    uint64_t num11[1] = {1};
    uint64_t num22[1] = {1}; 
    uint64_t num111[2] = {1, 1};
    uint64_t num222[2] = {1, 1}; 
 
 
    i1 = bksmt_int_init_lst(num1, 2, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num2, 2, BKSMT_INT_NEG);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != BKSMT_INT_POS)
        return 1;

    if (size != 2)
        return 2;

    if (num3[0] != BKSMT_INT_SL_MAX || num3[1] != 0)
        return 3;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num11, 1, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num22, 1, BKSMT_INT_NEG);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != BKSMT_INT_POS)
        return 4;

    if (size != 1)
        return 5;

    if (num3[0] != 0)
        return 6;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num111, 1, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num222, 1, BKSMT_INT_NEG);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != BKSMT_INT_POS)
        return 7;

    if (size != 1)
        return 8;

    if (num3[0] != 0)
        return 9;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num2, 2, BKSMT_INT_POS);
    i2 = bksmt_int_init_lst(num1, 2, BKSMT_INT_NEG);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != BKSMT_INT_NEG)
        return 10;

    if (size != 2)
        return 11;

    if (num3[0] != BKSMT_INT_SL_MAX || num3[1] != 0)
        return 12;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);




}
