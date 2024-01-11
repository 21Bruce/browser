#include "chacha20.h"
#include "chacha20_blk.h"

#include <math.h>
#include <stdint.h>

void 
bksmt_chacha20(unsigned char key[32], unsigned char nonce[12], uint32_t counter, unsigned char *in, int len, unsigned char *out)
{
    unsigned char kblk[64];
    int blklen, i, j;


    blklen = len/64;

    for (i = 0; i < blklen; i++) {
        /* generate chacha20 block */ 
        bksmt_chacha20_blk(key, nonce, counter + i, kblk); 
        /* xor plaintext block with chacha20 block and store */
        for (j = 0; j < 64; j++)
            out[i * 64 + j] = in[i * 64 + j] ^ kblk[j];
    }

    /* special case if input is not multiple of blk size */ 
    if ((len % 64) != 0) {
        /* generate chacha20 block */ 
        bksmt_chacha20_blk(key, nonce, counter + blklen, kblk); 
        /* xor plaintext block with chacha20 block and store */
        for (j = 0; j < len % 64; j++)
            out[blklen * 64 + j] = in[blklen * 64 + j] ^ kblk[j];
    }


}
