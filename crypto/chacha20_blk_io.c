#include "chacha20_blk_io.h"
#include "../lib/pack.h"

#include <stdint.h>

void 
bksmt_chacha20_blk_in(unsigned char key[32], unsigned char nonce[12], uint32_t blockcount, uint32_t state[16]) 
{
    int i;

    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;

    /* unpack key into 4 byte LE uints */
    for(i = 0; i < 8; i++)
        state[4 + i] = bksmt_packle32(key + 4*i);

    /* insert block # */
    state[12] = blockcount;

    /* unpack nonce into 4 byte LE uints */
    for(i = 0; i < 3; i++)
        state[13 + i] = bksmt_packle32(nonce + 4*i);
}


void 
bksmt_chacha20_blk_out(uint32_t state[16], unsigned char out[64])
{
    int i;
    for (i = 0; i < 16; i++)
        bksmt_unpackle32(state[i], out + 4*i);
}
