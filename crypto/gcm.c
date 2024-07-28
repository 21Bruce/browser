#include <stdint.h> 
#include <inttypes.h> 
#include <stdio.h> 
#include <string.h> 
#include <assert.h> 
#include <stdlib.h>

#include "aes.h"
#include "../lib/pack.h"
#include "../lib/math.h"
#include "../lib/xmalloc.h"



/* NIST 800-38d 6.2 */
static void gcm_inc32(uint64_t [2]);

/* NIST 800-38d 6.3 */
/* interpreted as 128 bit LE */
static void gcm_blkmul(uint64_t [2], uint64_t [2], uint64_t [2]);

/* NIST 800-38d 6.4 */
static void gcm_aes_ghash(uint64_t [2], uint64_t *, size_t, uint64_t [2]);

/* NIST 800-38d 6.5 */
static void gcm_aes_gctr(unsigned char [16], uint64_t [2], uint64_t *, size_t, uint64_t *);

static void blk_rshift(uint64_t [2], int);

static void bytetoblk(unsigned char [16], uint64_t [2]);

static void blktobyte(uint64_t [2], unsigned char [16]);

static void blk_xor(uint64_t [2], uint64_t [2]);

static void byte_xor_str(unsigned char *, unsigned char *, unsigned char *, size_t);

static int blk_getb(uint64_t [2], int);

static void print_blk_bin(uint64_t [2]);

static void 
print_blk_bin(uint64_t blk[2])
{
    int i;
    fprintf(stderr, "0b"); 
    for(i = 127; i > 63; i--)
        fprintf(stderr, "%d", blk_getb(blk, i)); 

    fprintf(stderr, "|"); 

    for(i = 63; i > -1; i--)
        fprintf(stderr, "%d", blk_getb(blk, i)); 
}

void
bksmt_gcm_aes_ae(unsigned char key[16], unsigned char iv[12], unsigned char *p, size_t plen, unsigned char *a, size_t alen, unsigned char *c, unsigned char *t, size_t tlen)
{ 
    uint64_t x1[2], x2[2], x3[2], x4[2], m1_64[2]; 
    uint64_t i, st;
    unsigned char m1[16] = 
    { 
        0x08, 0x00, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
        0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C
    };
    unsigned char k1[16] = 
    {
        0xAD, 0x7A, 0x2B, 0xD0, 0x3E, 0xAC, 0x83, 0x5A,
        0x6F, 0x62, 0x0F, 0xDC, 0xB5, 0x06, 0xB3, 0x45
    };

    memset(m1_64, 0, sizeof m1_64); 
    memset(k1, 0, sizeof k1);

    x1[1] = 0x0;
    x1[0] = 0x1;

    x2[1] = 0;
    x2[0] = 0;

    gcm_aes_gctr(k1, x1, m1_64, 2, x2);
    fprintf(stderr, "0x%llx%llx\n", x2[1], x2[0]);
 

    x2[1] = 0x0000000000000000; 
    x2[0] = 0x00000000FFFFFFFF; 

    print_blk_bin(x2);
    fprintf(stderr, "\n");
    for(i = 0; i < st; i++) {
        gcm_inc32(x2);
        print_blk_bin(x2);
        fprintf(stderr, "\n");
   }
 


} 


static void 
gcm_aes_gctr(unsigned char key[16], uint64_t icb[2], uint64_t *msg, size_t msglen, uint64_t *out)
{
    size_t n, i, extra;
    int j;
    uint64_t cb[2];
    unsigned char cbb[16], ciphcbb[16];

    /* set cb to icb */
    memcpy(cb, icb, sizeof cb);
    for (i = 0; i < msglen/2; i++) {
        /* convert cb to bytes */
        blktobyte(cb, cbb); 
        /* encrypt the byte form of counter blk */
//        for (j = 0; j < 16; j++)
//            fprintf(stderr, "%.02x", cbb[j]);
//        fprintf(stderr, "\n");
//        fprintf(stderr, "\n");
        bksmt_aes_128(cbb, key, ciphcbb);
//        for (j = 0; j < 16; j++)
//            fprintf(stderr, "%.02x", ciphcbb[j]);
//        fprintf(stderr, "\n");

        /* convert ciphered counter blk bytes back to blk */
        bytetoblk(ciphcbb, cb); 
        /* xor byte counter blk w/ x blk, store in y blk */
        blk_xor(out + i * 2, msg + i * 2);
        blk_xor(out + i * 2, cb);
        /* increment counter block */
        gcm_inc32(cb);
    }

}

static void 
blk_rshift(uint64_t blk[2], int i)
{

    if (i == 0) {
        return;
    } else if (i < 64) {
        blk[0] = (blk[0] >> i) | (blk[1] << (64 - i));
        blk[1] >>= i;
    } else {
        blk[0] = blk[1] >> (i-64);
        blk[1] = 0;
    }
}

static void 
blk_xor(uint64_t blk1[2], uint64_t blk2[2])
{
    blk1[0] ^= blk2[0];
    blk1[1] ^= blk2[1];
}

static int 
blk_getb(uint64_t blk[2], int i)
{
    uint64_t sto;

    sto = 1;
    if (i < 64) {
        sto = blk[0] & (sto << i);
        return sto >> i;    
     } else {
        sto = blk[1] & (sto << (i-64));
        return sto >> (i-64);    
     }
}

static void
gcm_inc32(uint64_t blk[2])
{
    uint32_t sto;
    sto = blk[0] & 0x00000000FFFFFFFF;
    sto += 1;
    blk[0] = (blk[0] & 0xFFFFFFFF00000000) | ((uint64_t)sto & 0x00000000FFFFFFFF);
}


static void 
gcm_blkmul(uint64_t x[2], uint64_t y[2], uint64_t z[2])
{
    uint64_t r[2], v[2], ztmp[2]; 
    int i, xi, v0;

    /* r const */

    r[1] = 0xE100000000000000;
    r[0] = 0; 

    /* set z to zero */
    memset(ztmp, 0, 2 * sizeof *ztmp);

    /* set v to y */
    memcpy(v, y, 2 * sizeof *y);
    
    for(i = 0; i < 128; i++) {
        /* get the ith bit of x */
        xi = blk_getb(x, 127-i); 
        /* get the LSB of v */
        v0 = blk_getb(v, 0);
        /* if xi == 1, spec says xor z = z ^ v*/
        if (xi) blk_xor(ztmp, v);
        /* v =  v >> 1*/
        blk_rshift(v, 1);
        /* if LSB(v) == 1, spec says v = v ^ r*/
        if (v0) blk_xor(v, r);
    }
    memcpy(z, ztmp, 2 * sizeof *z);
    
}

static void
gcm_aes_ghash(uint64_t hsubkey[2], uint64_t *msg, size_t msglen, uint64_t out[2])
{
    size_t i;

    /* zero out */
    out[0] = 0;
    out[1] = 0;

    for(i = 0; i < msglen/2; i++) {
        blk_xor(out, msg + i*2); 
        gcm_blkmul(out, hsubkey, out); 
    }
}


static void 
bytetoblk(unsigned char bytes[16], uint64_t blk[2])
{
    blk[1] = bksmt_packbe64(bytes);    
    blk[0] = bksmt_packbe64(bytes + 8);    
}

static void 
blktobyte(uint64_t blk[2], unsigned char bytes[16])
{
    bksmt_unpackbe64(blk[1], bytes);    
    bksmt_unpackbe64(blk[0], bytes + 8);    
}


static void 
byte_xor_str(unsigned char *y, unsigned char *x1, unsigned char *x2, size_t bytelen) {
    int i;
    for (i = 0; i < bytelen; i++)
        y[i] = x1[i] ^ x2[i];
}
