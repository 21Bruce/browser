#ifndef __BKSMT_CRYPTO_SHA_H__
#define __BKSMT_CRYPTO_SHA_H__

#include <stdint.h>

/* each returns a byte array interpreted as a BE-int */

/* Args: msg, len, output */
void bksmt_sha1(unsigned char *, uint64_t, unsigned char[20]);

/* Args: msg, len, output */
void bksmt_sha256(unsigned char *, uint64_t, unsigned char[32]);
                                                         
/* Args: msg, len, output */
void bksmt_sha224(unsigned char *, uint64_t, unsigned char[28]);
                                                         
/* Args: msg, len, output */
void bksmt_sha512(unsigned char *, uint64_t, unsigned char[64]);
                                                         
/* Args: msg, len, output */
void bksmt_sha384(unsigned char *, uint64_t, unsigned char[48]);

/* Args: msg, len, output */
void bksmt_sha512t224(unsigned char *, uint64_t, unsigned char[28]);
                                                             
/* Args: msg, len, output */
void bksmt_sha512t256(unsigned char *, uint64_t, unsigned char[32]);

/* this allows us to continuously hash different buffers into the same digest */

struct bksmt_sha256_ctx {
    uint32_t hash[8];
    unsigned char blk[64];
    uint64_t len;
    int aib;
};

struct bksmt_sha512_ctx {
    uint64_t hash[8];
    unsigned char blk[128];
    uint64_t len;
    int aib;
};

void bksmt_sha224_ctx_init(struct bksmt_sha256_ctx *);

void bksmt_sha256_ctx_init(struct bksmt_sha256_ctx *);

void bksmt_sha224_ctx_hash(struct bksmt_sha256_ctx *, unsigned char *, uint64_t);

void bksmt_sha256_ctx_hash(struct bksmt_sha256_ctx *, unsigned char *, uint64_t);

void bksmt_sha256_ctx_finish(struct bksmt_sha256_ctx *, unsigned char [32]);

void bksmt_sha224_ctx_finish(struct bksmt_sha256_ctx *, unsigned char [28]);

void bksmt_sha512_ctx_init(struct bksmt_sha512_ctx *);

void bksmt_sha384_ctx_init(struct bksmt_sha512_ctx *);

void bksmt_sha512t224_ctx_init(struct bksmt_sha512_ctx *);

void bksmt_sha512t256_ctx_init(struct bksmt_sha512_ctx *);

void bksmt_sha512_ctx_hash(struct bksmt_sha512_ctx *, unsigned char *, uint64_t);

void bksmt_sha384_ctx_hash(struct bksmt_sha512_ctx *, unsigned char *, uint64_t);

void bksmt_sha512t224_ctx_hash(struct bksmt_sha512_ctx *, unsigned char *, uint64_t);

void bksmt_sha512t256_ctx_hash(struct bksmt_sha512_ctx *, unsigned char *, uint64_t);

void bksmt_sha512_ctx_finish(struct bksmt_sha512_ctx *, unsigned char [64]);

void bksmt_sha384_ctx_finish(struct bksmt_sha512_ctx *, unsigned char [48]);

void bksmt_sha512t224_ctx_finish(struct bksmt_sha512_ctx *, unsigned char [28]);

void bksmt_sha512t256_ctx_finish(struct bksmt_sha512_ctx *, unsigned char [32]);

#endif /* __BKSMT_CRYPTO_SHA_H__ */
