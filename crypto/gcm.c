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
/* interpreted as 128 bit LE polynomial */
static void gcm_blkmul(uint64_t [2], uint64_t [2], uint64_t [2]);

/* NIST 800-38d 6.4 */

/* generic ghash func */
static void gcm_aes_ghash(uint64_t [2], unsigned char *, size_t , unsigned char *, size_t, uint64_t [2]);

/* NIST 800-38d 6.5 */
static void gcm_aes_gctr(unsigned char [16], uint64_t [2], unsigned  char *, size_t, unsigned char *);

static void blk_rshift(uint64_t [2], int);

static void bytetoblk(unsigned char [16], uint64_t [2]);

static void blktobyte(uint64_t [2], unsigned char [16]);

static void blk_xor(uint64_t [2], uint64_t [2]);

static int blk_getb(uint64_t [2], int);

static void print_blk_bin(uint64_t [2]);

static void prep_j0(unsigned char [12], uint64_t [2]);

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

static void 
prep_j0(unsigned char iv[12], uint64_t j0[2]) 
{
    uint64_t x1, x2, x3, x4; 
    j0[1] = bksmt_packbe64(iv);
    x1 = iv[8];
    x1 <<= 56;
    x2 = iv[9];
    x2 <<= 48;
    x3 = iv[10];
    x3 <<= 40;
    x4 = iv[11];
    x4 <<= 32;
    j0[0] = x1 + x2 + x3 + x4 + 0x1;
}

void
bksmt_gcm_aes_128_ae(unsigned char key[16], unsigned char iv[12], unsigned char *p, size_t plen, unsigned char *a, size_t alen, unsigned char *c, unsigned char *t, size_t tlen)
{ 
    unsigned char zerob[16], hskb[16], tbig[16], digestb[16];
    uint64_t j0[2], j1[2], hsubk[2], digest[2]; 

    prep_j0(iv, j0);   

    j1[0] = j0[0];
    j1[1] = j0[1];
 
    gcm_inc32(j1);

    gcm_aes_gctr(key, j1, p, plen, c);

    memset(zerob, 0, 16);
    bksmt_aes_128(zerob, key, hskb);
    bytetoblk(hskb, hsubk);

    gcm_aes_ghash(hsubk, a, alen, c, plen, digest);

    blktobyte(digest, digestb);
    gcm_aes_gctr(key, j0, digestb, 16, tbig);


    memcpy(t, tbig, tlen);
} 


static void 
gcm_aes_gctr(unsigned char key[16], uint64_t icb[2], unsigned char *msg, size_t msglen, unsigned char *out)
{
    size_t n, i, extra;
    int j;
    uint64_t cb[2], ciphcb[2], mblk[2], oblk[2], sto[2];
    unsigned char cbb[16], ciphcbb[16], ebyte[16];

    memset(ebyte, 0, 16);


    /* set cb to icb */
    memcpy(cb, icb, sizeof cb);
    for (i = 0; i < msglen/16; i++) {
        oblk[0] = 0;
        oblk[1] = 0;
        /* convert cb to bytes */
        blktobyte(cb, cbb); 
        /* encrypt the byte form of counter blk */
        bksmt_aes_128(cbb, key, ciphcbb);
        /* convert ciphered counter blk bytes back to blk */
        bytetoblk(ciphcbb, ciphcb); 
        bytetoblk(msg + i * 16, mblk); 
        /* xor byte counter blk w/ x blk, store in y blk */
        blk_xor(oblk, mblk);
        blk_xor(oblk, ciphcb);
        blktobyte(oblk, out + i * 16);
        /* increment counter block */
        gcm_inc32(cb);
    }

    if (msglen % 16 != 0) {
        memcpy(ebyte, msg + rounddown(msglen, 16), msglen % 16);
        bytetoblk(ebyte, mblk);
        oblk[0] = 0;
        oblk[1] = 0;
        /* convert cb to bytes */
        blktobyte(cb, cbb); 
        /* encrypt the byte form of counter blk */
        bksmt_aes_128(cbb, key, ciphcbb);
        /* convert ciphered counter blk bytes back to blk */
        bytetoblk(ciphcbb, ciphcb); 
        /* xor byte counter blk w/ x blk, store in y blk */
        blk_xor(oblk, mblk);
        blk_xor(oblk, ciphcb);
        blktobyte(oblk, ebyte);
        memcpy(out + i * 16, ebyte, msglen % 16);
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
gcm_aes_ghash(uint64_t hsubkey[2], unsigned char *a, size_t alen, unsigned char *c, size_t clen, uint64_t out[2])
{
    size_t i;
    uint64_t sto[2];
    unsigned char ebyte[16];

    /* zero out extra byte space */
    memset(ebyte, 0, 16);

    /* zero out */
    out[0] = 0;
    out[1] = 0;

    /* hash the aad */
    for(i = 0; i < alen/16; i++) {
        bytetoblk(a + i*16, sto);
        blk_xor(out, sto); 
        gcm_blkmul(out, hsubkey, out); 
    }

    /* zero padded remainder */
    if (alen % 16 != 0) {
        memcpy(ebyte, a + rounddown(alen, 16), alen - rounddown(alen, 16));
        bytetoblk(ebyte, sto);
        blk_xor(out, sto); 
        gcm_blkmul(out, hsubkey, out); 
        memset(ebyte, 0, 16);
    } 

    /* hash the ciphertext */
    for(i = 0; i < clen/16; i++) {
        bytetoblk(c + i*16, sto);
        blk_xor(out, sto); 
        gcm_blkmul(out, hsubkey, out); 
    }

    /* zero padded remainder */
    if (clen % 16 != 0) {
        memcpy(ebyte, c + rounddown(clen, 16), clen - rounddown(clen, 16));
        bytetoblk(ebyte, sto);
        blk_xor(out, sto); 
        gcm_blkmul(out, hsubkey, out); 
        memset(ebyte, 0, 16);
    } 

    /* add bit lengths */
    sto[1] = alen * 8;
    sto[0] = clen * 8;
    blk_xor(out, sto); 
    gcm_blkmul(out, hsubkey, out); 
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

