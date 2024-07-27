#include "chacha20.h"
#include "../lib/pack.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#define ROTL(x,n) (((x) << (n)) | ((x) >> (((sizeof (x)) * 8) - (n))))

/* 
 * inputs: key, nonce, block #
 * outputs: ciphertext buffer 
 */
static void chacha20_blk(unsigned char [32], unsigned char [12], uint32_t, unsigned char [64]);

static void chacha20_round(uint32_t [16]);

static void chacha20_blk_qround(uint32_t *, uint32_t *, uint32_t *, uint32_t *);

static void chacha20_blk_in(unsigned char [32], unsigned char [12], uint32_t, uint32_t [16]);

static void chacha20_blk_out(uint32_t [16], unsigned char [64]);

void 
bksmt_chacha20(unsigned char key[32], unsigned char nonce[12], uint32_t counter, unsigned char *in, int len, unsigned char *out)
{
    unsigned char kblk[64];
    int blklen, i, j;


    blklen = len/64;

    for (i = 0; i < blklen; i++) {
        /* generate chacha20 block */ 
        chacha20_blk(key, nonce, counter + i, kblk); 
        /* xor plaintext block with chacha20 block and store */
        for (j = 0; j < 64; j++)
            out[i * 64 + j] = in[i * 64 + j] ^ kblk[j];
    }

    /* special case if input is not multiple of blk size */ 
    if ((len % 64) != 0) {
        /* generate chacha20 block */ 
        chacha20_blk(key, nonce, counter + blklen, kblk); 
        /* xor plaintext block with chacha20 block and store */
        for (j = 0; j < len % 64; j++)
            out[blklen * 64 + j] = in[blklen * 64 + j] ^ kblk[j];
    }


}

/* RFC 8439 Sec 2.3.1 */
static void 
chacha20_blk(unsigned char key[32], unsigned char nonce[12], uint32_t blockcount, unsigned char out[64])
{
    uint32_t state[16], statecp[16];
    int i;

    /* read inputs into state */
    chacha20_blk_in(key, nonce, blockcount, state);

    /* we need a copy of state for the end */
    memcpy(statecp, state, 64);

    /* 10 iterations of the round function */
    for(i = 0; i < 10; i++)
        chacha20_round(state);

    /* add initial state back */
    for(i = 0; i < 16; i++)
        state[i] += statecp[i];

    chacha20_blk_out(state, out);
}

/* RFC 8439 Sec 2.3.1 */
static void
chacha20_round(uint32_t state[16])
{
    chacha20_blk_qround(state + 0, state + 4, state + 8, state + 12);
    chacha20_blk_qround(state + 1, state + 5, state + 9, state + 13);
    chacha20_blk_qround(state + 2, state + 6, state + 10, state + 14);
    chacha20_blk_qround(state + 3, state + 7, state + 11, state + 15);
    chacha20_blk_qround(state + 0, state + 5, state + 10, state + 15);
    chacha20_blk_qround(state + 1, state + 6, state + 11, state + 12);
    chacha20_blk_qround(state + 2, state + 7, state + 8, state + 13);
    chacha20_blk_qround(state + 3, state + 4, state + 9, state + 14);
}

static void 
chacha20_blk_qround(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{
    *a += *b; *d ^= *a; *d = ROTL(*d, 16);
    *c += *d; *b ^= *c; *b = ROTL(*b, 12);
    *a += *b; *d ^= *a; *d = ROTL(*d, 8);
    *c += *d; *b ^= *c; *b = ROTL(*b, 7);
}

static void 
chacha20_blk_in(unsigned char key[32], unsigned char nonce[12], uint32_t blockcount, uint32_t state[16]) 
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


static void 
chacha20_blk_out(uint32_t state[16], unsigned char out[64])
{
    int i;
    for (i = 0; i < 16; i++)
        bksmt_unpackle32(state[i], out + 4*i);
}
