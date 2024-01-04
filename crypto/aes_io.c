#include "aes_io.h"

void
bksmt_aes_in(unsigned char in[16], unsigned char state[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            state[4*i + j] = in[4*j + i];
}

void
bksmt_aes_out(unsigned char state[16], unsigned char out[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
           out[i + 4 *j] = state[4*i + j]; 
}
