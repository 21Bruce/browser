#include "aes_op.h"
#include "aes_const.h"
#include "aes_poly.h"
#include <stdio.h> 

void
bksmt_aes_subbytes(unsigned char state[16])
{
    int r, c, i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++) {
            r = (state[4*i + j] & 0xF0) >> 4;
            c = state[4*i + j] & 0x0F;
            state[4*i + j] = aes_subbytes_lut[16*r + c];
        }
}

void
bksmt_aes_shiftrows(unsigned char state[16])
{
    unsigned char tmp;

    /* row 1, rotate one left */
    tmp = state[4];
    state[4] = state[5];
    state[5] = state[6];
    state[6] = state[7];
    state[7] = tmp;

    /* 
     * row 2, rotate two left, equivalent of swapping
     * 0,2 and 1,3 -> 8,10 and 9,11 in idx form
     */
    tmp = state[8];
    state[8] = state[10];
    state[10] = tmp;
    tmp = state[9];
    state[9] = state[11];
    state[11] = tmp;

    /* 
     * row 3, rotate three left, equivalent of 
     * rotate 1 right.
     */
    tmp = state[12];
    state[12] = state[15];
    state[15] = state[14];
    state[14] = state[13];
    state[13] = tmp;
}

void
bksmt_aes_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        bksmt_byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}

void 
bksmt_aes_inv_subbytes(unsigned char state[16])
{
    int r, c, i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++) {
            r = (state[4*i + j] & 0xF0) >> 4;
            c = state[4*i + j] & 0x0F;
            state[4*i + j] = aes_inv_subbytes_lut[16*r + c];
        }
}

void 
bksmt_aes_inv_shiftrows(unsigned char state[16])
{
    unsigned char temp;

    /* first row, one shift right */
    temp = state[4];
    state[4] = state[7];
    state[7] = state[6];
    state[6] = state[5];
    state[5] = temp;

    /* first row, two shift right */
    temp = state[8];
    state[8] = state[10];
    state[10] = temp;
    temp = state[9];
    state[9] = state[11];
    state[11] = temp;

    /* first row, three shift right, equivalent to one shift left */
    temp = state[12];
    state[12] = state[13];
    state[13] = state[14];
    state[14] = state[15];
    state[15] = temp;
}

void
bksmt_aes_inv_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_INV_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        bksmt_byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}


void 
bksmt_aes_addroundkey(unsigned char state[16], unsigned char roundkey[16])
{
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++)
        for (i = 0; i < 4; i++) 
            state[4 * j + i] ^= roundkey[4*i + j];
}

