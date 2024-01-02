#include "sha.h"

#include "sha_const.h"
#include "sha_pad.h"
#include "../lib/pack.h"
#include "../lib/math.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint32_t sha1ft(int , uint32_t, uint32_t, uint32_t);

static void sha1w_gen(uint32_t[80], unsigned char *, int);

void
bksmt_sha1(unsigned char ret[20], unsigned char *src, int len) 
{
    uint32_t *prs, work[5], w[80], tmp, hash[5] = SHA1_INIT;
    int blks, i, t, plen;

    /* pad input */
    prs = bksmt_sha256_pad(src, len);

    /* calc byte length for padded input (essentially size of prs) */
    plen = bksmt_sha256_pad_len(len);

    /* prs length in # of 512 bit groups */
    blks = plen/64;

    /* iterate through 512-bit chunks of message */
    for(i = 0; i < blks; i++) {
        /* generate message schedule */
        sha1w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 20);

        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp = (ROTL(work[0], 5) + sha1ft(t, work[1], work[2], work[3]) + work[4] + sha1_const_lut[t] + w[t]) % (4294967296);
            work[4] = work[3];
            work[3] = work[2];
            work[2] = ROTL(work[1], 30);
            work[1] = work[0];
            work[0] = tmp;
        }

        /* add work to hash */
        for (t = 0; t < 5; t++) {
            hash[t] = (hash[t] + work[t]) % (4294967296);
        }
    }

    free(prs);

    for (i = 0; i < 5; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}


static uint32_t 
sha1ft(int t, uint32_t x, uint32_t y, uint32_t z)
{
    if (0 <= t && t <= 19)
        return CH(x,y,z);
    if (20 <= t && t <= 39)
        return PARITY(x,y,z);
    if (40 <= t && t <= 59)
        return MAJ(x,y,z);
    if (60 <= t && t <= 79)
        return PARITY(x,y,z);
}

static void 
sha1w_gen(uint32_t ret[80], unsigned char *pmsg, int i) 
{
    int t;
    for (t = 0; t < 80; t++) {
        if (t <= 15)
            ret[t] = bksmt_packbe32(pmsg + 64 * i + 4 * t);
        else
            ret[t] = ROTL(ret[t-3] ^ ret[t-8] ^ ret[t-14] ^ ret[t-16], 1);
    }
}
