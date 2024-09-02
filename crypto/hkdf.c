#include "hkdf.h"
#include "hmac.h"
#include "../lib/math.h"

#include <string.h>
#include <assert.h>

void 
bksmt_sha256_hkdf_extract_ctx_init(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[32];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 32);
        bksmt_sha256_hmac_ctx_init(&ctx->hmctx, zero, 32);
    } else {
        bksmt_sha256_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha256_hkdf_extract_ctx_append(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha256_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha256_hkdf_extract_ctx_finish(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char ret[32])
{
    bksmt_sha256_hmac_ctx_finish(&ctx->hmctx, ret);
}

void 
bksmt_sha224_hkdf_extract_ctx_init(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[28];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 28);
        bksmt_sha224_hmac_ctx_init(&ctx->hmctx, zero, 28);
    } else {
        bksmt_sha224_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha224_hkdf_extract_ctx_append(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha224_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha224_hkdf_extract_ctx_finish(struct bksmt_sha256_hkdf_extract_ctx *ctx, unsigned char ret[28])
{
    bksmt_sha224_hmac_ctx_finish(&ctx->hmctx, ret);
}

void 
bksmt_sha512_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[64];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 64);
        bksmt_sha512_hmac_ctx_init(&ctx->hmctx, zero, 64);
    } else {
        bksmt_sha512_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha512_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha512_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha512_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char ret[64])
{
    bksmt_sha512_hmac_ctx_finish(&ctx->hmctx, ret);
}

void 
bksmt_sha384_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[48];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 48);
        bksmt_sha384_hmac_ctx_init(&ctx->hmctx, zero, 48);
    } else {
        bksmt_sha384_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha384_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha384_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha384_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char ret[48])
{
    bksmt_sha384_hmac_ctx_finish(&ctx->hmctx, ret);
}

void 
bksmt_sha512t224_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[28];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 28);
        bksmt_sha512t224_hmac_ctx_init(&ctx->hmctx, zero, 28);
    } else {
        bksmt_sha512t224_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha512t224_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha512t224_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha512t224_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char ret[28])
{
    bksmt_sha512t224_hmac_ctx_finish(&ctx->hmctx, ret);
}

void 
bksmt_sha512t256_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *salt, uint64_t saltlen)
{
    unsigned char zero[32];
    if (salt == NULL || saltlen == 0) {
        bzero(zero, 32);
        bksmt_sha512t256_hmac_ctx_init(&ctx->hmctx, zero, 32);
    } else {
        bksmt_sha512t256_hmac_ctx_init(&ctx->hmctx, salt, saltlen);
    }
}

void 
bksmt_sha512t256_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char *ikmbuf, uint64_t ikmbuflen)
{
    bksmt_sha512t256_hmac_ctx_append(&ctx->hmctx, ikmbuf, ikmbuflen);
}

void 
bksmt_sha512t256_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *ctx, unsigned char ret[32])
{
    bksmt_sha512t256_hmac_ctx_finish(&ctx->hmctx, ret);
}


void 
bksmt_sha256_hkdf_expand(unsigned char prk[32], uint64_t prklen, unsigned char *info, uint64_t infolen, unsigned char *out, uint64_t outlen)
{
    struct bksmt_sha256_hmac_ctx ctx;
    uint64_t n, i, extra;
    unsigned char thash[32], incbuf;

    n = outlen / 32;
    extra = outlen - n*32;    

    incbuf = 1;
    bksmt_sha256_hmac_ctx_init(&ctx, prk, prklen);
    bksmt_sha256_hmac_ctx_append(&ctx, info, infolen);
    bksmt_sha256_hmac_ctx_append(&ctx, &incbuf, 1);
    bksmt_sha256_hmac_ctx_finish(&ctx, out);

    for (i = 1; i < n; i++) {
        incbuf++;
        bksmt_sha256_hmac_ctx_init(&ctx, prk, prklen);
        bksmt_sha256_hmac_ctx_append(&ctx, out + (i-1)*32, 32);
        bksmt_sha256_hmac_ctx_append(&ctx, info, infolen);
        bksmt_sha256_hmac_ctx_append(&ctx, &incbuf, 1);
        bksmt_sha256_hmac_ctx_finish(&ctx, out + i*32);
    }

    if (extra != 0) {
        incbuf++;
        bksmt_sha256_hmac_ctx_init(&ctx, prk, prklen);
        bksmt_sha256_hmac_ctx_append(&ctx, out + (i-1)*32, 32);
        bksmt_sha256_hmac_ctx_append(&ctx, info, infolen);
        bksmt_sha256_hmac_ctx_append(&ctx, &incbuf, 1);
        bksmt_sha256_hmac_ctx_finish(&ctx, thash);

        memcpy(out + i * 32, thash, extra);
    }
 
}

