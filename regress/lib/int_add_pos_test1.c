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

    uint64_t num1[2] = {1, 1}; 
    uint64_t num2[2] = {0, 1};
    uint64_t num11[3] = {0, 1, 1}; 
    uint64_t num22[2] = {0, 1};
    uint64_t num111[1] = {BKSMT_INT_SL_MAX}; 
    uint64_t num222[1] = {1};
    uint64_t num1111[1] = {BKSMT_INT_SL_MAX}; 
    uint64_t num2222[1] = {2};



    i1 = bksmt_int_init_lst(num1, 2, 1);
    i2 = bksmt_int_init_lst(num2, 2, 1);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != 1)
        return 1;

    if (size != 2)
        return 2;

    if (num3[0] != 1 || num3[1] != 2)
        return 3;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num11, 3, 1);
    i2 = bksmt_int_init_lst(num22, 2, 1);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != 1)
        return 4;

    if (size != 3)
        return 5;

    if (num3[0] != 0 || num3[1] != 2 || num3[2] != 1)
        return 6;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num11, 3, 1);
    i2 = bksmt_int_init_lst(num22, 2, 1);
    bksmt_int_adds(i2, i1);    
    bksmt_int_lst_rep(i2, &num3, &size, &sign);

    if (sign != 1)
        return 7;

    if (size != 3)
        return 8;

    if (num3[0] != 0 || num3[1] != 2 || num3[2] != 1)
        return 9;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num111, 1, 1);
    i2 = bksmt_int_init_lst(num222, 1, 1);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != 1)
        return 10;

    if (size != 2)
        return 11;

    if (num3[0] != 0 || num3[1] != 1)
        return 12;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    i1 = bksmt_int_init_lst(num222, 1, 1);
    i2 = bksmt_int_init_lst(num111, 1, 1);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != 1)
        return 13;

    if (size != 2)
        return 14;

    if (num3[0] != 0 || num3[1] != 1)
        return 15;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);

    
    i1 = bksmt_int_init_lst(num2222, 1, 1);
    i2 = bksmt_int_init_lst(num1111, 1, 1);
    bksmt_int_adds(i1, i2);    
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (sign != 1)
        return 16;

    if (size != 2)
        return 17;

    if (num3[0] != 1 || num3[1] != 1)
        return 18;

    bksmt_int_free(i1);
    bksmt_int_free(i2);
    free(num3);



}
