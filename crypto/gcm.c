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
static void gcm_aes_ghash(unsigned char [16], unsigned char *, size_t, uint64_t [16]);

/* NIST 800-38d 6.5 */
static void gcm_aes_gctr(unsigned char [16], uint64_t [2], unsigned char *, size_t, unsigned char *);

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
    uint64_t x1[2], x2[2], x3[2];
    int i;
   
    x1[1] = 0x0000000000000000; 
    x1[0] = 3; 

    x2[1] = 0x0000000000000000; 
    x2[0] = 2; 

    gcm_blkmul(x2, x1, x3);

    print_blk_bin(x1);
    fprintf(stderr, "\n");
    print_blk_bin(x2);
    fprintf(stderr, "\n");
    print_blk_bin(x3);
    fprintf(stderr, "\n");

} 


static void 
gcm_aes_gctr(unsigned char key[16], uint64_t icb[2], unsigned char *x, size_t xblen, unsigned char *y)
{
    size_t n, i, extra;
    uint64_t cb[2];
    unsigned char cbb[16], ciphcbb[16];

    /* set cb to icb */
    memcpy(cb, icb, 2 * sizeof *cb);

    /* calc # of blks in input */
    n = roundup(xblen, 128)/128;

    for (i = 0; i < n; i++) {
        /* convert cb to bytes */
        blktobyte(cb, cbb); 
        /* encrypt the byte form of counter blk */
        bksmt_aes_128(cbb, key, ciphcbb);
        /* xor byte counter blk w/ x blk, store in y blk */
        if (i != n-1) {
            byte_xor_str(y + i * 16, x + i * 16, ciphcbb, 16);
        } else {
            extra = (xblen - rounddown(xblen, 128))/8;
            byte_xor_str(y + i * 16, x + i * 16, ciphcbb, extra);
        }
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
    sto = ((uint32_t)(blk[0] & 0x0000000011111111) + 1);
    blk[0] = (blk[0] & 0x1111111100000000) || ((uint64_t)(sto) & 0x0000000011111111);
}


static void 
gcm_blkmul(uint64_t x[2], uint64_t y[2], uint64_t z[2])
{
    uint64_t r[2], v[2]; 
    int i, xi, v0;

    /* r const */
//    r[0] = 0;
//    r[1] = 0xE100000000000000;

      r[0] = 0x78;
      r[1] = 0;
    
    /* set z to zero */
    memset(z, 0, 2 * sizeof *z);

    /* set v to y */
    memcpy(v, y, 2 * sizeof *y);
    
    for(i = 0; i < 128; i++) {
        fprintf(stderr, "start z: ");
        print_blk_bin(z);
        fprintf(stderr, "\n");
        fprintf(stderr, "start v: ");
        print_blk_bin(v);
        fprintf(stderr, "\n");
        /* get the ith bit of x */
        xi = blk_getb(x, 127-i); 
        fprintf(stderr, "%dth bit of x: %d\n", i, xi);
        /* get the LSB of v */
        v0 = blk_getb(v, 0);
        fprintf(stderr, "%dth bit of v: %d\n", 0, v0);
        /* if xi == 1, spec says xor z = z ^ v*/
        if (xi){
            blk_xor(z, v);
            fprintf(stderr, "z ^= v: ");
            print_blk_bin(z);
            fprintf(stderr, "\n");
        }
        /* v =  v >> 1*/
        blk_rshift(v, 1);
        fprintf(stderr, "v >>= 1: ");
        print_blk_bin(v);
        fprintf(stderr, "\n");
        /* if LSB(v) == 1, spec says v = v ^ r*/
        if (v0) {
            blk_xor(v, r);
            fprintf(stderr, "v ^= r: ");
            print_blk_bin(v);
            fprintf(stderr, "\n");
        }
    }

}

//static void
//gcm_aes_ghash(unsigned char key[16], uint64_t *msg, size_t msglen, uint64_t out[2])
//{
//    size_t i;
//    unsigned char zero[16], hsubkb[16];
//    uint64_t hsubk[2], xi[2];
//
//    /* generate hash subkey */
//    memset(zero, 0, 16 * sizeof *zero);
//    bksmt_aes_128(zero, key, hsubkb);
//    bytetoblk(hsubk, hsubkb);
//
//    /* zero out */
//    out[0] = 0;
//    out[1] = 0;
//
//    for(i = 0; i < msglen/2; i++) {
//        blk_xor(out, xi + i*2); 
//        gcm_blkmul(out, hsubk, out); 
//    }
//}


static void 
bytetoblk(unsigned char bytes[16], uint64_t blk[2])
{
    blk[0] = bksmt_packle64(bytes);    
    blk[1] = bksmt_packle64(bytes + 8);    
}

static void 
blktobyte(uint64_t blk[2], unsigned char bytes[16])
{
    bksmt_unpackle64(blk[0], bytes);    
    bksmt_unpackle64(blk[1], bytes + 8);    
}


static void 
byte_xor_str(unsigned char *y, unsigned char *x1, unsigned char *x2, size_t bytelen) {
    int i;
    for (i = 0; i < bytelen; i++)
        y[i] = x1[i] ^ x2[i];
}
