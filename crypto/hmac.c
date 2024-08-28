#include "hmac.h"
#include "sha.h"
#include "../lib/xmalloc.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HMAC_IPAD_64 {              \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36          \
}

#define HMAC_OPAD_64 {              \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C          \
}

#define HMAC_IPAD_128 {             \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36, 0x36, 0x36,   \
    0x36, 0x36, 0x36                \
}

#define HMAC_OPAD_128 {             \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C, 0x5C, 0x5C,   \
    0x5C, 0x5C, 0x5C                \
}

static void byte_xor(unsigned char *, unsigned char *, uint64_t, unsigned char *);

void 
bksmt_sha256_hmac_ctx_init(struct bksmt_sha256_hmac_ctx *ctx, unsigned char *key, uint64_t klen) 
{
    unsigned char khash[32], kipad[64] = HMAC_IPAD_64, kopad[64] = HMAC_OPAD_64;

    /* init sha256 ctx */
    bksmt_sha256_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 64) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha256(key, klen, khash);
        byte_xor(khash, kipad, 32, kipad);
        byte_xor(khash, kopad, 32, ctx->opad);
        memcpy(ctx->opad, kopad, 64);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 64);
    }

    /* RFC 2104: hash padded key */
    bksmt_sha256_ctx_hash(&ctx->sctx, kipad, 64); 
}

void 
bksmt_sha224_hmac_ctx_init(struct bksmt_sha256_hmac_ctx *ctx, unsigned char *key, uint64_t klen) 
{
    unsigned char khash[28], kipad[64] = HMAC_IPAD_64, kopad[64] = HMAC_OPAD_64;

    /* init sha224 ctx */
    bksmt_sha224_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 64) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha224(key, klen, khash);
        byte_xor(khash, kipad, 28, kipad);
        byte_xor(khash, kopad, 28, ctx->opad);
        memcpy(ctx->opad, kopad, 64);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 64);
    }

    /* RFC 2104: hash padded key */
    bksmt_sha224_ctx_hash(&ctx->sctx, kipad, 64); 
}

void 
bksmt_sha512_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *key, uint64_t klen) 
{
    unsigned char khash[64], kipad[128] = HMAC_IPAD_128, kopad[128] = HMAC_OPAD_128;

    /* init sha224 ctx */
    bksmt_sha512_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 128) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha384(key, klen, khash);
        byte_xor(khash, kipad, 64, kipad);
        byte_xor(khash, kopad, 64, ctx->opad);
        memcpy(ctx->opad, kopad, 128);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 128);
    }

    /* RFC 2104: hash padded key */
    bksmt_sha512_ctx_hash(&ctx->sctx, kipad, 128); 
}

void 
bksmt_sha384_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *key, uint64_t klen)
{
    unsigned char khash[48], kipad[128] = HMAC_IPAD_128, kopad[128] = HMAC_OPAD_128;

    /* init sha384 ctx */
    bksmt_sha384_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 128) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha384(key, klen, khash);
        byte_xor(khash, kipad, 48, kipad);
        byte_xor(khash, kopad, 48, ctx->opad);
        memcpy(ctx->opad, kopad, 128);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 128);
    }

    /* RFC 2104: hash padded key */
    bksmt_sha384_ctx_hash(&ctx->sctx, kipad, 128); 
}

void 
bksmt_sha512t224_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *key, uint64_t klen)
{
    unsigned char khash[28], kipad[128] = HMAC_IPAD_128, kopad[128] = HMAC_OPAD_128;

    /* init sha512t224 ctx */
    bksmt_sha512t224_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 128) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha512t224(key, klen, khash);
        byte_xor(khash, kipad, 28, kipad);
        byte_xor(khash, kopad, 28, ctx->opad);
        memcpy(ctx->opad, kopad, 128);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 128);
    }

    /* RFC 2104: hash padded key */
    bksmt_sha512t224_ctx_hash(&ctx->sctx, kipad, 128); 
}

void 
bksmt_sha512t256_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *key, uint64_t klen)
{
    unsigned char khash[32], kipad[128] = HMAC_IPAD_128, kopad[128] = HMAC_OPAD_128;

    /* init sha512t256 ctx */
    bksmt_sha512t256_ctx_init(&ctx->sctx); 

    /* RFC 2104: xoring ipad to key */
    if (klen > 128) { 
        /* RFC 2104: if key len > block size, must hash down */
        bksmt_sha512t256(key, klen, khash);
        byte_xor(khash, kipad, 32, kipad);
        byte_xor(khash, kopad, 32, ctx->opad);
        memcpy(ctx->opad, kopad, 128);
    } else {
        byte_xor(key, kipad, klen, kipad);
        byte_xor(key, kopad, klen, kopad);
        memcpy(ctx->opad, kopad, 128);
    }
    bksmt_sha512t256_ctx_hash(&ctx->sctx, kipad, 128); 
}

void 
bksmt_sha256_hmac_ctx_append(struct bksmt_sha256_hmac_ctx *ctx, unsigned char *buf, uint64_t len)
{
    bksmt_sha256_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha224_hmac_ctx_append(struct bksmt_sha256_hmac_ctx *ctx, unsigned char *buf, uint64_t len) 
{
    bksmt_sha224_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha512_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *buf, uint64_t len)
{
    bksmt_sha512_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha384_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *buf, uint64_t len)
{
    bksmt_sha384_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha512t224_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *buf, uint64_t len)
{
    bksmt_sha384_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha512t256_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *ctx, unsigned char *buf, uint64_t len)
{
    bksmt_sha512t256_ctx_hash(&ctx->sctx, buf, len); 
}

void 
bksmt_sha256_hmac_ctx_finish(struct bksmt_sha256_hmac_ctx *ctx, unsigned char ret[32])
{
    unsigned char fhash[32];

    /* generate hash of ipad concat with message */
    bksmt_sha256_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha256_ctx_init(&ctx->sctx);
    bksmt_sha256_ctx_hash(&ctx->sctx, ctx->opad, 64);
    bksmt_sha256_ctx_hash(&ctx->sctx, fhash, 32);
    /* output the hmac */
    bksmt_sha256_ctx_finish(&ctx->sctx, ret);
}

void 
bksmt_sha224_hmac_ctx_finish(struct bksmt_sha256_hmac_ctx *ctx, unsigned char ret[28])
{
    unsigned char fhash[28];

    /* generate hash of ipad concat with message */
    bksmt_sha224_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha224_ctx_init(&ctx->sctx);
    bksmt_sha224_ctx_hash(&ctx->sctx, ctx->opad, 64);
    bksmt_sha224_ctx_hash(&ctx->sctx, fhash, 28);
    /* output the hmac */
    bksmt_sha224_ctx_finish(&ctx->sctx, ret);
}

void 
bksmt_sha512_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *ctx, unsigned char ret[64])
{
    unsigned char fhash[64];

    /* generate hash of ipad concat with message */
    bksmt_sha512_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha512_ctx_init(&ctx->sctx);
    bksmt_sha512_ctx_hash(&ctx->sctx, ctx->opad, 128);
    bksmt_sha512_ctx_hash(&ctx->sctx, fhash, 64);
    /* output the hmac */
    bksmt_sha512_ctx_finish(&ctx->sctx, ret);

}

void 
bksmt_sha384_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *ctx, unsigned char ret[48])
{    
    unsigned char fhash[48];

    /* generate hash of ipad concat with message */
    bksmt_sha384_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha384_ctx_init(&ctx->sctx);
    bksmt_sha384_ctx_hash(&ctx->sctx, ctx->opad, 128);
    bksmt_sha384_ctx_hash(&ctx->sctx, fhash, 64);
    /* output the hmac */
    bksmt_sha384_ctx_finish(&ctx->sctx, ret);
}

void 
bksmt_sha512t224_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *ctx, unsigned char ret[28])
{
    unsigned char fhash[28];

    /* generate hash of ipad concat with message */
    bksmt_sha512t224_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha512t224_ctx_init(&ctx->sctx);
    bksmt_sha512t224_ctx_hash(&ctx->sctx, ctx->opad, 128);
    bksmt_sha512t224_ctx_hash(&ctx->sctx, fhash, 28);
    /* output the hmac */
    bksmt_sha512t224_ctx_finish(&ctx->sctx, ret);
}

void 
bksmt_sha512t256_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *ctx, unsigned char ret[32])
{
    unsigned char fhash[32];

    /* generate hash of ipad concat with message */
    bksmt_sha512t256_ctx_finish(&ctx->sctx, fhash);
    /* reinit the hash ctx to hash the opad concat with previous hash */
    bksmt_sha512t256_ctx_init(&ctx->sctx);
    bksmt_sha512t256_ctx_hash(&ctx->sctx, ctx->opad, 128);
    bksmt_sha512t256_ctx_hash(&ctx->sctx, fhash, 32);
    /* output the hmac */
    bksmt_sha512t256_ctx_finish(&ctx->sctx, ret);
}

static void 
byte_xor(unsigned char *x, unsigned char *y, uint64_t len, unsigned char *out)
{
    uint64_t i;

    for (i = 0; i < len; i++)
        out[i] = x[i] ^ y[i];
}
