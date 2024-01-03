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

static void sha256w_gen(uint32_t[65], unsigned char *, int); 

static void sha512w_gen(uint64_t[80], unsigned char *, int); 

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
            tmp = ROTL(work[0], 5) + sha1ft(t, work[1], work[2], work[3]) + work[4] + sha1_const_lut[t] + w[t];
            work[4] = work[3];
            work[3] = work[2];
            work[2] = ROTL(work[1], 30);
            work[1] = work[0];
            work[0] = tmp;
        }

        /* add work to hash */
        for (t = 0; t < 5; t++) {
            hash[t] = hash[t] + work[t];
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

void
bksmt_sha256(unsigned char ret[32], unsigned char *src, int len) 
{
    uint32_t *prs, work[8], w[65], tmp1, tmp2, hash[8] = SHA256_INIT;
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
        sha256w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 32);
 
        /* generate new work vars */
        for (t = 0; t < 64; t++) {
            tmp1 = work[7] + SUM2561(work[4]) + CH(work[4], work[5], work[6]) + sha256_const_lut[t] + w[t];
            tmp2 = SUM2560(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 8; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

void
bksmt_sha224(unsigned char ret[28], unsigned char *src, int len) 
{
    uint32_t *prs, work[8], w[65], tmp1, tmp2, hash[8] = SHA224_INIT;
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
        sha256w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 32);
 
        /* generate new work vars */
        for (t = 0; t < 64; t++) {
            tmp1 = work[7] + SUM2561(work[4]) + CH(work[4], work[5], work[6]) + sha256_const_lut[t] + w[t];
            tmp2 = SUM2560(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 7; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

static void 
sha256w_gen(uint32_t ret[65], unsigned char *pmsg, int i) 
{
    int t;
    for (t = 0; t < 64; t++) {
        if (t <= 15)
            ret[t] = bksmt_packbe32(pmsg + 64 * i + 4 * t);
        else
            ret[t] = (SIG2561(ret[t-2]) + ret[t-7] + SIG2560(ret[t-15]) + ret[t-16]);
    }
}

void
bksmt_sha512(unsigned char ret[64], unsigned char *src, int len) 
{
    uint64_t *prs, work[8], w[80], tmp1, tmp2, hash[8] = SHA512_INIT;
    int blks, i, t, plen;

    /* pad input */
    prs = bksmt_sha512_pad(src, len);

    /* calc byte length for padded input (essentially size of prs) */
    plen = bksmt_sha512_pad_len(len);

    /* prs length in # of 1024 bit groups */
    blks = plen/128;

    /* iterate through 1024-bit chunks of message */
    for(i = 0; i < blks; i++) {
        /* generate message schedule */
        sha512w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 64);
 
        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp1 = work[7] + SUM5121(work[4]) + CH(work[4], work[5], work[6]) + sha512_const_lut[t] + w[t];
            tmp2 = SUM5120(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 8; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
}

void
bksmt_sha384(unsigned char ret[48], unsigned char *src, int len) 
{
    uint64_t *prs, work[8], w[80], tmp1, tmp2, hash[8] = SHA384_INIT;
    int blks, i, t, plen;

    /* pad input */
    prs = bksmt_sha512_pad(src, len);

    /* calc byte length for padded input (essentially size of prs) */
    plen = bksmt_sha512_pad_len(len);

    /* prs length in # of 1024 bit groups */
    blks = plen/128;

    /* iterate through 1024-bit chunks of message */
    for(i = 0; i < blks; i++) {
        /* generate message schedule */
        sha512w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 64);
 
        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp1 = work[7] + SUM5121(work[4]) + CH(work[4], work[5], work[6]) + sha512_const_lut[t] + w[t];
            tmp2 = SUM5120(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 6; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
}

void
bksmt_sha512t224(unsigned char ret[28], unsigned char *src, int len) 
{
    uint64_t *prs, work[8], w[80], tmp1, tmp2, hash[8] = SHA512224_INIT;
    int blks, i, t, plen;
    unsigned char tmpstr[8];

    /* pad input */
    prs = bksmt_sha512_pad(src, len);

    /* calc byte length for padded input (essentially size of prs) */
    plen = bksmt_sha512_pad_len(len);

    /* prs length in # of 1024 bit groups */
    blks = plen/128;

    /* iterate through 1024-bit chunks of message */
    for(i = 0; i < blks; i++) {
        /* generate message schedule */
        sha512w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 64);
 
        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp1 = work[7] + SUM5121(work[4]) + CH(work[4], work[5], work[6]) + sha512_const_lut[t] + w[t];
            tmp2 = SUM5120(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 3; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
    bksmt_unpackbe64(hash[3], tmpstr);

    memcpy(ret + 24, tmpstr, 4);
}

void
bksmt_sha512t256(unsigned char ret[32], unsigned char *src, int len) 
{
    uint64_t *prs, work[8], w[80], tmp1, tmp2, hash[8] = SHA512256_INIT;
    int blks, i, t, plen;

    /* pad input */
    prs = bksmt_sha512_pad(src, len);

    /* calc byte length for padded input (essentially size of prs) */
    plen = bksmt_sha512_pad_len(len);

    /* prs length in # of 1024 bit groups */
    blks = plen/128;

    /* iterate through 1024-bit chunks of message */
    for(i = 0; i < blks; i++) {
        /* generate message schedule */
        sha512w_gen(w, prs, i);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 64);
 
        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp1 = work[7] + SUM5121(work[4]) + CH(work[4], work[5], work[6]) + sha512_const_lut[t] + w[t];
            tmp2 = SUM5120(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + tmp1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = tmp1 + tmp2;
       }

        /* add work to hash */
        for (t = 0; t < 8; t++) {
            hash[t] = hash[t] + work[t];
        }
    }

    free(prs);

    for (i = 0; i < 4; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 

}
static void 
sha512w_gen(uint64_t ret[80], unsigned char *pmsg, int i) 
{
    int t;
    for (t = 0; t < 80; t++) {
        if (t <= 15)
            ret[t] = bksmt_packbe64(pmsg + 128 * i + 8 * t);
        else
            ret[t] = (SIG5121(ret[t-2]) + ret[t-7] + SIG5120(ret[t-15]) + ret[t-16]);
    }
}
