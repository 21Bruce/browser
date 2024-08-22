#include "sha.h"

#include "../lib/pack.h"
#include "../lib/math.h"
#include "../lib/xmalloc.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* SHS Section 3.2 */
#define ROTL(x,n) (((x) << (n)) | ((x) >> (((sizeof (x)) * 8) - (n))))
#define ROTR(x,n) (((x) >> (n)) | ((x) << (((sizeof (x)) * 8) - (n))))
#define SHR(x,n) ((x) >> (n))
#define ADD2(x,y) (((x) + (y)) % (1 << sizeof (x)))

/* SHS Section 4.1 */
#define CH(x,y,z) (((x) & (y)) ^ ((~(x)) & (z)))
#define PARITY(x,y,z) ((x) ^ (y) ^ (z)) 
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define SUM2560(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define SUM2561(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))
#define SIG2560(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ SHR((x), 3))
#define SIG2561(x) (ROTR((x), 17) ^ ROTR((x), 19) ^  SHR((x), 10))

#define SUM5120(x) (ROTR((x), 28) ^ ROTR((x), 34) ^ ROTR((x), 39))
#define SUM5121(x) (ROTR((x), 14) ^ ROTR((x), 18) ^ ROTR((x), 41))
#define SIG5120(x) (ROTR((x), 1) ^ ROTR((x), 8) ^ SHR((x), 7))
#define SIG5121(x) (ROTR((x), 19) ^ ROTR((x), 61) ^  SHR((x), 6))

/* SHS Section 4.2 */
static uint32_t sha1_const_lut[80] = {
   0x5a827999, /* 0  */
   0x5a827999, /* 1  */
   0x5a827999, /* 2  */
   0x5a827999, /* 3  */
   0x5a827999, /* 4  */
   0x5a827999, /* 5  */
   0x5a827999, /* 6  */
   0x5a827999, /* 7  */
   0x5a827999, /* 8  */
   0x5a827999, /* 9  */
   0x5a827999, /* 10 */
   0x5a827999, /* 11 */
   0x5a827999, /* 12 */
   0x5a827999, /* 13 */
   0x5a827999, /* 14 */
   0x5a827999, /* 15 */
   0x5a827999, /* 16 */
   0x5a827999, /* 17 */
   0x5a827999, /* 18 */
   0x5a827999, /* 19 */
   0x6ed9eba1, /* 20 */
   0x6ed9eba1, /* 21 */
   0x6ed9eba1, /* 22 */
   0x6ed9eba1, /* 23 */
   0x6ed9eba1, /* 24 */
   0x6ed9eba1, /* 25 */
   0x6ed9eba1, /* 26 */
   0x6ed9eba1, /* 27 */
   0x6ed9eba1, /* 28 */
   0x6ed9eba1, /* 29 */
   0x6ed9eba1, /* 30 */
   0x6ed9eba1, /* 31 */
   0x6ed9eba1, /* 32 */
   0x6ed9eba1, /* 33 */
   0x6ed9eba1, /* 34 */
   0x6ed9eba1, /* 35 */
   0x6ed9eba1, /* 36 */
   0x6ed9eba1, /* 37 */
   0x6ed9eba1, /* 38 */
   0x6ed9eba1, /* 39 */
   0x8f1bbcdc, /* 40 */
   0x8f1bbcdc, /* 41 */
   0x8f1bbcdc, /* 42 */
   0x8f1bbcdc, /* 43 */
   0x8f1bbcdc, /* 44 */
   0x8f1bbcdc, /* 45 */
   0x8f1bbcdc, /* 46 */
   0x8f1bbcdc, /* 47 */
   0x8f1bbcdc, /* 48 */
   0x8f1bbcdc, /* 49 */
   0x8f1bbcdc, /* 50 */
   0x8f1bbcdc, /* 51 */
   0x8f1bbcdc, /* 52 */
   0x8f1bbcdc, /* 53 */
   0x8f1bbcdc, /* 54 */
   0x8f1bbcdc, /* 55 */
   0x8f1bbcdc, /* 56 */
   0x8f1bbcdc, /* 57 */
   0x8f1bbcdc, /* 58 */
   0x8f1bbcdc, /* 59 */
   0xca62c1d6, /* 60 */
   0xca62c1d6, /* 61 */
   0xca62c1d6, /* 62 */
   0xca62c1d6, /* 63 */
   0xca62c1d6, /* 64 */
   0xca62c1d6, /* 65 */
   0xca62c1d6, /* 66 */
   0xca62c1d6, /* 67 */
   0xca62c1d6, /* 68 */
   0xca62c1d6, /* 69 */
   0xca62c1d6, /* 70 */
   0xca62c1d6, /* 71 */
   0xca62c1d6, /* 72 */
   0xca62c1d6, /* 73 */
   0xca62c1d6, /* 74 */
   0xca62c1d6, /* 75 */
   0xca62c1d6, /* 76 */
   0xca62c1d6, /* 77 */
   0xca62c1d6, /* 78 */
   0xca62c1d6, /* 79 */
};

static uint32_t sha256_const_lut[64] = {
    0x428a2f98, /* 0  */
    0x71374491, /* 1  */
    0xb5c0fbcf, /* 2  */
    0xe9b5dba5, /* 3  */
    0x3956c25b, /* 4  */
    0x59f111f1, /* 5  */
    0x923f82a4, /* 6  */
    0xab1c5ed5, /* 7  */
    0xd807aa98, /* 8  */
    0x12835b01, /* 9  */
    0x243185be, /* 10 */
    0x550c7dc3, /* 11 */
    0x72be5d74, /* 12 */
    0x80deb1fe, /* 13 */
    0x9bdc06a7, /* 14 */
    0xc19bf174, /* 15 */
    0xe49b69c1, /* 16 */
    0xefbe4786, /* 17 */
    0x0fc19dc6, /* 18 */
    0x240ca1cc, /* 19 */
    0x2de92c6f, /* 20 */
    0x4a7484aa, /* 21 */
    0x5cb0a9dc, /* 22 */
    0x76f988da, /* 23 */
    0x983e5152, /* 24 */
    0xa831c66d, /* 25 */
    0xb00327c8, /* 26 */
    0xbf597fc7, /* 27 */
    0xc6e00bf3, /* 28 */
    0xd5a79147, /* 29 */
    0x06ca6351, /* 30 */
    0x14292967, /* 31 */
    0x27b70a85, /* 32 */
    0x2e1b2138, /* 33 */
    0x4d2c6dfc, /* 34 */
    0x53380d13, /* 35 */
    0x650a7354, /* 36 */
    0x766a0abb, /* 37 */
    0x81c2c92e, /* 38 */
    0x92722c85, /* 39 */
    0xa2bfe8a1, /* 40 */
    0xa81a664b, /* 41 */
    0xc24b8b70, /* 42 */
    0xc76c51a3, /* 43 */
    0xd192e819, /* 44 */
    0xd6990624, /* 45 */
    0xf40e3585, /* 46 */
    0x106aa070, /* 47 */
    0x19a4c116, /* 48 */
    0x1e376c08, /* 49 */
    0x2748774c, /* 50 */
    0x34b0bcb5, /* 51 */
    0x391c0cb3, /* 52 */
    0x4ed8aa4a, /* 53 */
    0x5b9cca4f, /* 54 */
    0x682e6ff3, /* 55 */
    0x748f82ee, /* 56 */
    0x78a5636f, /* 57 */
    0x84c87814, /* 58 */
    0x8cc70208, /* 59 */
    0x90befffa, /* 60 */
    0xa4506ceb, /* 61 */
    0xbef9a3f7, /* 62 */
    0xc67178f2, /* 63 */
};

static uint64_t sha512_const_lut[80] = {
    0x428a2f98d728ae22, /* 0  */
    0x7137449123ef65cd, /* 1  */
    0xb5c0fbcfec4d3b2f, /* 2  */
    0xe9b5dba58189dbbc, /* 3  */
    0x3956c25bf348b538, /* 4  */
    0x59f111f1b605d019, /* 5  */
    0x923f82a4af194f9b, /* 6  */
    0xab1c5ed5da6d8118, /* 7  */
    0xd807aa98a3030242, /* 8  */
    0x12835b0145706fbe, /* 9  */
    0x243185be4ee4b28c, /* 10 */
    0x550c7dc3d5ffb4e2, /* 11 */
    0x72be5d74f27b896f, /* 12 */
    0x80deb1fe3b1696b1, /* 13 */
    0x9bdc06a725c71235, /* 14 */
    0xc19bf174cf692694, /* 15 */
    0xe49b69c19ef14ad2, /* 16 */
    0xefbe4786384f25e3, /* 17 */
    0x0fc19dc68b8cd5b5, /* 18 */
    0x240ca1cc77ac9c65, /* 19 */
    0x2de92c6f592b0275, /* 20 */
    0x4a7484aa6ea6e483, /* 21 */
    0x5cb0a9dcbd41fbd4, /* 22 */
    0x76f988da831153b5, /* 23 */
    0x983e5152ee66dfab, /* 24 */
    0xa831c66d2db43210, /* 25 */
    0xb00327c898fb213f, /* 26 */
    0xbf597fc7beef0ee4, /* 27 */
    0xc6e00bf33da88fc2, /* 28 */
    0xd5a79147930aa725, /* 29 */
    0x06ca6351e003826f, /* 30 */
    0x142929670a0e6e70, /* 31 */
    0x27b70a8546d22ffc, /* 32 */
    0x2e1b21385c26c926, /* 33 */
    0x4d2c6dfc5ac42aed, /* 34 */
    0x53380d139d95b3df, /* 35 */
    0x650a73548baf63de, /* 36 */
    0x766a0abb3c77b2a8, /* 37 */
    0x81c2c92e47edaee6, /* 38 */
    0x92722c851482353b, /* 39 */
    0xa2bfe8a14cf10364, /* 40 */
    0xa81a664bbc423001, /* 41 */
    0xc24b8b70d0f89791, /* 42 */
    0xc76c51a30654be30, /* 43 */
    0xd192e819d6ef5218, /* 44 */
    0xd69906245565a910, /* 45 */
    0xf40e35855771202a, /* 46 */
    0x106aa07032bbd1b8, /* 47 */
    0x19a4c116b8d2d0c8, /* 48 */
    0x1e376c085141ab53, /* 49 */
    0x2748774cdf8eeb99, /* 50 */
    0x34b0bcb5e19b48a8, /* 51 */
    0x391c0cb3c5c95a63, /* 52 */
    0x4ed8aa4ae3418acb, /* 53 */
    0x5b9cca4f7763e373, /* 54 */
    0x682e6ff3d6b2b8a3, /* 55 */
    0x748f82ee5defb2fc, /* 56 */
    0x78a5636f43172f60, /* 57 */
    0x84c87814a1f0ab72, /* 58 */
    0x8cc702081a6439ec, /* 59 */
    0x90befffa23631e28, /* 60 */
    0xa4506cebde82bde9, /* 61 */
    0xbef9a3f7b2c67915, /* 62 */
    0xc67178f2e372532b, /* 63 */
    0xca273eceea26619c, /* 64 */
    0xd186b8c721c0c207, /* 65 */
    0xeada7dd6cde0eb1e, /* 66 */
    0xf57d4f7fee6ed178, /* 67 */
    0x06f067aa72176fba, /* 68 */
    0x0a637dc5a2c898a6, /* 69 */
    0x113f9804bef90dae, /* 70 */
    0x1b710b35131c471b, /* 71 */
    0x28db77f523047d84, /* 72 */
    0x32caab7b40c72493, /* 73 */
    0x3c9ebe0a15c9bebc, /* 74 */
    0x431d67c49c100d4c, /* 75 */
    0x4cc5d4becb3e42b6, /* 76 */
    0x597f299cfc657e2a, /* 77 */
    0x5fcb6fab3ad6faec, /* 78 */
    0x6c44198c4a475817, /* 79 */
};

/* SHS Section 5.3 */
#define SHA1_INIT { \
    0x67452301,     \
    0xefcdab89,     \
    0x98badcfe,     \
    0x10325476,     \
    0xc3d2e1f0,     \
}

#define SHA224_INIT { \
    0xc1059ed8,       \
    0x367cd507,       \
    0x3070dd17,       \
    0xf70e5939,       \
    0xffc00b31,       \
    0x68581511,       \
    0x64f98fa7,       \
    0xbefa4fa4,       \
}

#define SHA256_INIT { \
    0x6a09e667,       \
    0xbb67ae85,       \
    0x3c6ef372,       \
    0xa54ff53a,       \
    0x510e527f,       \
    0x9b05688c,       \
    0x1f83d9ab,       \
    0x5be0cd19,       \
}

#define SHA384_INIT {   \
    0xcbbb9d5dc1059ed8, \
    0x629a292a367cd507, \
    0x9159015a3070dd17, \
    0x152fecd8f70e5939, \
    0x67332667ffc00b31, \
    0x8eb44a8768581511, \
    0xdb0c2e0d64f98fa7, \
    0x47b5481dbefa4fa4, \
}

#define SHA512_INIT {   \
    0x6a09e667f3bcc908, \
    0xbb67ae8584caa73b, \
    0x3c6ef372fe94f82b, \
    0xa54ff53a5f1d36f1, \
    0x510e527fade682d1, \
    0x9b05688c2b3e6c1f, \
    0x1f83d9abfb41bd6b, \
    0x5be0cd19137e2179, \
}

#define SHA512224_INIT { \
    0x8C3D37C819544DA2,  \
    0x73E1996689DCD4D6,  \
    0x1DFAB7AE32FF9C82,  \
    0x679DD514582F9FCF,  \
    0x0F6D2B697BD44DA8,  \
    0x77E36F7304C48942,  \
    0x3F9D85A86A1D36C8,  \
    0x1112E6AD91D692A1,  \
}

#define SHA512256_INIT { \
    0x22312194FC2BF72C,  \
    0x9F555FA3C84C64C2,  \
    0x2393B86B6F53B151,  \
    0x963877195940EABD,  \
    0x96283EE2A88EFFE3,  \
    0xBE5E1E2553863992,  \
    0x2B0199FC2C85B8AA,  \
    0x0EB72DDC81C52CA2,  \
}

static uint32_t sha1_ft(int , uint32_t, uint32_t, uint32_t);

static void sha1_w_gen(uint32_t[80], unsigned char *, unsigned char [64], unsigned char [64], uint64_t, uint64_t);

static void sha256_w_gen(uint32_t[65], unsigned char [64]);

static void sha512_w_gen(uint64_t [80], unsigned char *, unsigned char [128], unsigned char [128], uint64_t, uint64_t);

static void sha256_pad_gen(unsigned char *, uint64_t, unsigned char[64], unsigned char[64], uint64_t *);

static void sha256_multi_pad_gen(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[64], unsigned char[64], uint64_t *);

static void sha256_work_round(uint32_t [8], unsigned char [64]);

static void sha512_pad_gen(unsigned char *, uint64_t, unsigned char[128], unsigned char[128], uint64_t *);

static void sha256_hash_gen(unsigned char *, uint64_t, uint32_t [8]);

static void sha512_hash_gen(unsigned char *, uint64_t, uint64_t [8]);
 
static void sha256_multi_hash_gen(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, uint32_t [8]);

static void 
sha256_multi_pad_gen(unsigned char *front, uint64_t flen, unsigned char *src, uint64_t len, unsigned char *back, uint64_t blen, unsigned char opad1[64], unsigned char opad2[64], uint64_t *blkf)
{
    uint64_t blks, extra, extrat, bitlen, fullen;

    /* calculate the full length */
    fullen = flen + len + blen;

    blks = fullen / 64;
    extra = fullen % 64;

    /* length in bits */
    bitlen = fullen * 8;

    /* copy in bits from the whole msg into last block */
    if (extra <= blen) { 
        memcpy(opad1, back + blen - extra, extra);
    } else if (extra <= blen + len) {
        memcpy(opad1, src + len - extra + blen, extra - blen);
        memcpy(opad1 + extra - blen, back, blen);
    } else {
        memcpy(opad1, front + flen - extra + blen + len, extra - blen - len);
        memcpy(opad1 + extra - len - blen, src, len);
        memcpy(opad1 + extra - blen, back, blen);
    }

    memset(opad1 + extra + 1, 0, 63 - extra);
    opad1[extra] = 0x80; 

    if (extra < 56) {
        /* if we have enough space for the 8 bit block, we only need one pad */
        bksmt_unpackbe64(bitlen, opad1 + 56); 
        *blkf = blks + 1;
    } else {
        /* else, we need a whole new block to put the length in */
        memset(opad2, 0, 56);
        bksmt_unpackbe64(bitlen, opad2 + 56); 
        *blkf = blks + 2;
    }

}

static void 
sha256_pad_gen(unsigned char *src, uint64_t len, unsigned char opad1[64], unsigned char opad2[64], uint64_t *blkf)
{
    uint64_t blks, extra, blen;

    blks = len / 64;
    extra = len % 64;

    /* length in bits */
    blen = len * 8;

    /* construct last block */
    memcpy(opad1, src + len - extra, extra);
    memset(opad1 + extra + 1, 0, 63 - extra);
    opad1[extra] = 0x80; 

    /* write binary rep of len to end */

    if (extra < 56) {
        /* if we have enough space for the 8 bit block, we only need one pad */
        bksmt_unpackbe64(blen, opad1 + 56); 
        *blkf = blks + 1;
    } else {
        /* else, we need a whole new block to put the length in */
        memset(opad2, 0, 56);
        bksmt_unpackbe64(blen, opad2 + 56); 
        *blkf = blks + 2;
    }
}

static void 
sha512_pad_gen(unsigned char *src, uint64_t len, unsigned char opad1[128], unsigned char opad2[128], uint64_t *blkf )
{
    uint64_t blks, extra, blen;

    blks = len / 128;
    extra = len % 128;

    /* length in bits */
    blen = len * 8;

    /* construct last block */
    memcpy(opad1, src + len - extra, extra);
    memset(opad1 + extra + 1, 0, 127 - extra);
    opad1[extra] = 0x80; 

    /* write binary rep of len to end */

    if (extra < 112) {
        /* if we have enough space for the 8 bit block, we only need one pad */
        bksmt_unpackbe64(blen, opad1 + 120); 
        *blkf = blks + 1;
    } else {
        /* else, we need a whole new block to put the length in */
        memset(opad2, 0, 120);
        bksmt_unpackbe64(blen, opad2 + 120); 
        *blkf = blks + 2;
    }
}

void
bksmt_sha1(unsigned char *src, uint64_t len, unsigned char ret[20]) 
{
    uint32_t work[5], w[80], tmp, hash[5] = SHA1_INIT;
    uint64_t blks, i, t, extra, blen, blkf;
    unsigned char opad1[64], opad2[64];

    blks = len / 64;

    /* generate extra pad block */
    sha256_pad_gen(src, len, opad1, opad2, &blkf);

    /* iterate through 512-bit chunks of message */
    for(i = 0; i < blkf; i++) {
        /* generate message schedule */
        sha1_w_gen(w, src, opad1, opad2, i, blks);

        /* fill work vars w/ current hash */
        memcpy(work, hash, 20);

        /* generate new work vars */
        for (t = 0; t < 80; t++) {
            tmp = ROTL(work[0], 5) + sha1_ft(t, work[1], work[2], work[3]) + work[4] + sha1_const_lut[t] + w[t];
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

    for (i = 0; i < 5; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

static uint32_t 
sha1_ft(int t, uint32_t x, uint32_t y, uint32_t z)
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
sha1_w_gen(uint32_t ret[80], unsigned char *pmsg, unsigned char lpad1[64], unsigned char lpad2[64], uint64_t i, uint64_t blen) 
{
    int t;
    for (t = 0; t < 80; t++) {
        if (t <= 15) {
            if (i == blen)
                ret[t] = bksmt_packbe32(lpad1 + 4 * t);
            else if (i == blen + 1)
                ret[t] = bksmt_packbe32(lpad2 + 4 * t);
            else
                ret[t] = bksmt_packbe32(pmsg + 64 * i + 4 * t);
        }
        else
            ret[t] = ROTL(ret[t-3] ^ ret[t-8] ^ ret[t-14] ^ ret[t-16], 1);
    }
}

void
bksmt_sha256(unsigned char *src, uint64_t len, unsigned char ret[32]) 
{
    uint64_t i;
    uint32_t hash[8] = SHA256_INIT;

    sha256_hash_gen(src, len, hash);

    for (i = 0; i < 8; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

void
bksmt_sha256_multi(unsigned char *front, uint64_t flen, unsigned char *src, uint64_t len, unsigned char *back, uint64_t blen, unsigned char ret[32]) 
{
    uint64_t i;
    uint32_t hash[8] = SHA256_INIT;

    sha256_multi_hash_gen(front, flen, src, len, back, blen, hash);

    for (i = 0; i < 8; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

void
bksmt_sha224(unsigned char *src, uint64_t len, unsigned char ret[28]) 
{
    uint32_t hash[8] = SHA224_INIT;
    uint64_t i;

    sha256_hash_gen(src, len, hash);

    for (i = 0; i < 7; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

void
bksmt_sha224_multi(unsigned char *front, uint64_t flen, unsigned char *src, uint64_t len, unsigned char *back, uint64_t blen, unsigned char ret[28]) 
{
    uint32_t hash[8] = SHA224_INIT;
    uint64_t i;

    sha256_multi_hash_gen(front, flen, src, len, back, blen, hash);

    for (i = 0; i < 7; i++) 
        bksmt_unpackbe32(hash[i], ret + i * 4); 
}

static void 
sha256_multi_hash_gen(unsigned char *front, uint64_t flen, unsigned char *src, uint64_t len, unsigned char *back, uint64_t blen, uint32_t hash[8])
{
    uint64_t blks, blkfr, extrafr, blksr, extrasr, blkba, extraba, i, fullen, blkf, off;
    unsigned char gblk[64], opad1[64], opad2[64];

    fullen = flen + len + blen;
    blks = fullen / 64;
    blkfr = flen / 64;
    extrafr = flen % 64;
     
    sha256_multi_pad_gen(front, flen, src, len, back, blen, opad1, opad2, &blkf);




    if (blks == 0)
        goto pad;

    for(i = 0; i < blkfr; i++) 
        sha256_work_round(hash, front + i * 64);

    if (extrafr + len + blen <= 64) 
        goto pad;

    if (extrafr != 0) {
        memcpy(gblk, front + flen - extrafr, extrafr);
        if (64 - extrafr <= len) {
            memcpy(gblk + extrafr, src, 64 - extrafr);
            off = 64 - extrafr;
            sha256_work_round(hash, gblk);
        } else {
            memcpy(gblk + extrafr, src, len);
            memcpy(gblk + extrafr + len, back, 64 - extrafr - len);
            off = 64 - extrafr - len;
            sha256_work_round(hash, gblk);
            goto back;
        } 
    } else {
        off = 0;
    }


    blksr = (len - off) / 64;
    extrasr = (len - off) % 64;


    for(i = 0; i < blksr; i++) 
        sha256_work_round(hash, src + i * 64 + off);


    if (extrasr + blen <= 64) 
        goto pad;

    if (extrasr != 0) {
        memcpy(gblk, src + off + i * blksr, extrasr);
        if (blen >= 64) {
            memcpy(gblk + extrasr, back, 64 - extrasr);
            off = 64 - extrasr;
            sha256_work_round(hash, gblk);
        } else {
            memcpy(gblk + extrasr, back, blen - extrasr);
            sha256_work_round(hash, gblk);
            goto pad;
        }
    } else {
        off = 0;
    }

back:

    blkba = (blen - off) / 64;

    for(i = 0; i < blkba; i++) 
        sha256_work_round(hash, back + i * 64 + off);

pad:
    sha256_work_round(hash, opad1);

    if (blkf - blks == 2)
        sha256_work_round(hash, opad2);
}

static void 
sha256_hash_gen(unsigned char *src, uint64_t len, uint32_t hash[8])
{
    uint64_t blks, i, extra, blen, blkf;
    unsigned char opad1[64], opad2[64];

    blks = len / 64;

    sha256_pad_gen(src, len, opad1, opad2, &blkf);

    for (int t = 0; t < 64; t++)
        fprintf(stderr, "%.02x ", opad1[t]);

    /* iterate through 512-bit chunks of message */
    for(i = 0; i < blks; i++) 
        sha256_work_round(hash, src + i * 64);

    sha256_work_round(hash, opad1);

    if (blkf - blks == 2)
        sha256_work_round(hash, opad2);
}

static void 
sha256_work_round(uint32_t hash[8], unsigned char cblk[64])
{
    uint32_t work[8], w[65], tmp1, tmp2;
    uint64_t t;

    /* generate message schedule */
    sha256_w_gen(w, cblk);
    
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

static void 
sha256_w_gen(uint32_t ret[65], unsigned char blk[64])
{
    int t;
    for (t = 0; t < 64; t++) {
        if (t <= 15) 
            ret[t] = bksmt_packbe32(blk + 4 * t);
        else
            ret[t] = (SIG2561(ret[t-2]) + ret[t-7] + SIG2560(ret[t-15]) + ret[t-16]);
    }
}

void
bksmt_sha512(unsigned char *src, uint64_t len, unsigned char ret[64]) 
{
    uint64_t i, hash[8] = SHA512_INIT;

    sha512_hash_gen(src, len, hash);

    for (i = 0; i < 8; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
}

void
bksmt_sha384(unsigned char *src, uint64_t len, unsigned char ret[48]) 
{
    uint64_t i, hash[8] = SHA384_INIT;

    sha512_hash_gen(src, len, hash);

    for (i = 0; i < 6; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
}

void
bksmt_sha512t224(unsigned char *src, uint64_t len, unsigned char ret[28]) 
{
    uint64_t i, hash[8] = SHA512224_INIT;
    unsigned char tmpstr[8];

    sha512_hash_gen(src, len, hash);

    for (i = 0; i < 3; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 
    bksmt_unpackbe64(hash[3], tmpstr);

    memcpy(ret + 24, tmpstr, 4);
}

void
bksmt_sha512t256(unsigned char *src, uint64_t len, unsigned char ret[32]) 
{
    uint64_t i, hash[8] = SHA512256_INIT;

    sha512_hash_gen(src, len, hash);

    for (i = 0; i < 4; i++) 
        bksmt_unpackbe64(hash[i], ret + i * 8); 

}

static void 
sha512_hash_gen(unsigned char *src, uint64_t len, uint64_t hash[8])
{
    uint64_t work[8], w[80], tmp1, tmp2, blks, i, t, extra, blen, blkf;
    unsigned char opad1[128], opad2[128];

    blks = len / 128;

    sha512_pad_gen(src, len, opad1, opad2, &blkf);

    /* iterate through 512-bit chunks of message */
    for(i = 0; i < blkf; i++) {
        /* generate message schedule */
        sha512_w_gen(w, src, opad1, opad2, i, blks);

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
}

static void 
sha512_w_gen(uint64_t ret[80], unsigned char *pmsg, unsigned char opad1[128], unsigned char opad2[128], uint64_t i, uint64_t blen) 
{
    int t;
    for (t = 0; t < 80; t++) {
        if (t <= 15) {
            if (i == blen)
                ret[t] = bksmt_packbe64(opad1 + 8 * t);
            else if (i == blen + 1)
                ret[t] = bksmt_packbe64(opad2 + 8 * t);
            else 
                ret[t] = bksmt_packbe64(pmsg + 128 * i + 8 * t);
        } else
            ret[t] = (SIG5121(ret[t-2]) + ret[t-7] + SIG5120(ret[t-15]) + ret[t-16]);
    }
}
