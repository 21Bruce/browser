#ifndef __BKSMT_HMAC_H__
#define __BKSMT_HMAC_H__

#include <stdint.h>
#include "sha.h" 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha256_hmac(unsigned char *, uint64_t ,  unsigned char *, uint64_t, unsigned char [32]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha224_hmac(unsigned char *, uint64_t ,  unsigned char * , uint64_t, unsigned char [28]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512_hmac(unsigned char *, uint64_t ,  unsigned char *, uint64_t, unsigned char [64]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha384_hmac(unsigned char *, uint64_t ,  unsigned char *, uint64_t, unsigned char [48]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512t224_hmac(unsigned char *, uint64_t ,  unsigned char *, uint64_t, unsigned char [28]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512t256_hmac(unsigned char *, uint64_t ,  unsigned char *, uint64_t, unsigned char [32]); 

struct bksmt_sha256_hmac_ctx {
    struct bksmt_sha256_ctx sctx;
    unsigned char opad[64];
};

struct bksmt_sha512_hmac_ctx {
    struct bksmt_sha512_ctx sctx;
    unsigned char opad[128];
};

/* Args: ctx, key, keylen */
void bksmt_sha256_hmac_ctx_init(struct bksmt_sha256_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, key, keylen */
void bksmt_sha224_hmac_ctx_init(struct bksmt_sha256_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, key, keylen */
void bksmt_sha512_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, key, keylen */
void bksmt_sha384_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, key, keylen */
void bksmt_sha512t224_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, key, keylen */
void bksmt_sha512t256_hmac_ctx_init(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha256_hmac_ctx_append(struct bksmt_sha256_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha224_hmac_ctx_append(struct bksmt_sha256_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha512_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha384_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha512t224_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, buf, buflen */
void bksmt_sha512t256_hmac_ctx_append(struct bksmt_sha512_hmac_ctx *, unsigned char *, uint64_t); 

/* Args: ctx, out */
void bksmt_sha256_hmac_ctx_finish(struct bksmt_sha256_hmac_ctx *, unsigned char [32]); 

/* Args: ctx, out */
void bksmt_sha224_hmac_ctx_finish(struct bksmt_sha256_hmac_ctx *, unsigned char [28]); 

/* Args: ctx, out */
void bksmt_sha512_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *, unsigned char [64]); 

/* Args: ctx, out */
void bksmt_sha384_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *, unsigned char [48]);

/* Args: ctx, out */
void bksmt_sha512t224_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *, unsigned char [28]); 

/* Args: ctx, out */
void bksmt_sha512t256_hmac_ctx_finish(struct bksmt_sha512_hmac_ctx *, unsigned char [32]); 



#endif /* __BKSMT_HMAC_H__ */
