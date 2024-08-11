#include "../../lib/int.h" 

#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 


int
main(void)
{
    uint64_t *num3;
    struct bksmt_int *i1, *i2;
    size_t size;
    int sign;

    uint64_t num1[2] = {0, 1}; 
    uint64_t num2[2] = {0, 3};
    uint64_t num4[2] = {1, 0}; 

    i1 = bksmt_int_init_lst(num1, 2, BKSMT_INT_POS);
    bksmt_int_rshifts(i1, 0);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (memcmp(num1, num3, 2) != 0) 
        return 1;

    if (size != 2) 
        return 2;

    if (sign != 1) 
        return 3;

    free(num3); 


    bksmt_int_rshifts(i1, 1);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (num3[0] != 0x8000000000000000) 
        return 4;

    if (size != BKSMT_INT_POS) 
        return 5;

    if (sign != 1) 
        return 6;

    free(num3); 
    bksmt_int_free(i1);

    i1 = bksmt_int_init_lst(num1, 2, BKSMT_INT_POS);
    bksmt_int_rshifts(i1, 64);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (num3[0] != 0x1) 
        return 7;

    if (size != 1) 
        return 8;

    if (sign != BKSMT_INT_POS) 
        return 9;

    free(num3); 
    bksmt_int_free(i1);

    i1 = bksmt_int_init_lst(num2, 2, BKSMT_INT_POS);
    bksmt_int_rshifts(i1, 65);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

    if (num3[0] != 0x1) 
        return 10;

    if (size != 1) 
        return 11;

    if (sign != BKSMT_INT_POS) 
        return 12;

    free(num3); 
    bksmt_int_free(i1);

    i1 = bksmt_int_init_lst(num4, 1, BKSMT_INT_POS);
    bksmt_int_lshifts(i1, 64);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

   if (num3[0] != 0x0 || num3[1] != 0x1) 
        return 11;

    if (size != 2) 
        return 12;

    if (sign != BKSMT_INT_POS) 
        return 13;

    free(num3); 

    bksmt_int_lshifts(i1, 65);
    bksmt_int_lst_rep(i1, &num3, &size, &sign);

   if (num3[0] != 0x0 || num3[1] != 0x0 || num3[2] != 0x2) 
        return 14;

    if (size != 3) 
        return 15;

    if (sign != BKSMT_INT_POS) 
        return 16;

    free(num3); 
 
    bksmt_int_free(i1);

    return 0;
}
