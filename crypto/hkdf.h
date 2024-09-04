#ifndef __BKSMT_HKDF_H__
#define __BKSMT_HKDF_H__

#include "sha.h"
#include "hmac.h"

struct bksmt_sha256_hkdf_extract_ctx {
    struct bksmt_sha256_hmac_ctx hmctx;
};

struct bksmt_sha512_hkdf_extract_ctx {
    struct bksmt_sha512_hmac_ctx hmctx;
};

/* extract ctx init functions. Args: ctx, salt, saltlen */

void bksmt_sha256_hkdf_extract_ctx_init(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha224_hkdf_extract_ctx_init(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha384_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512t224_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512t256_hkdf_extract_ctx_init(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

/* extract ctx append functions. Args: ctx, ikmbuf, ikmbuflen */

void bksmt_sha256_hkdf_extract_ctx_append(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha224_hkdf_extract_ctx_append(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha384_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512t224_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

void bksmt_sha512t256_hkdf_extract_ctx_append(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char *, uint64_t len);

/* extract ctx finish functions. Args: ctx, outbuf */

void bksmt_sha256_hkdf_extract_ctx_finish(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char [32]);

void bksmt_sha224_hkdf_extract_ctx_finish(struct bksmt_sha256_hkdf_extract_ctx *, unsigned char [28]);

void bksmt_sha512_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char [64]);

void bksmt_sha384_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char [48]);

void bksmt_sha512t224_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char [28]);

void bksmt_sha512t256_hkdf_extract_ctx_finish(struct bksmt_sha512_hkdf_extract_ctx *, unsigned char [32]);

/* expand prk, prklen, info, infolen, out, outlen */

void bksmt_sha256_hkdf_expand(unsigned char [32], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);

void bksmt_sha224_hkdf_expand(unsigned char [28], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);

void bksmt_sha512_hkdf_expand(unsigned char [64], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);

void bksmt_sha384_hkdf_expand(unsigned char [48], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);

void bksmt_sha512t224_hkdf_expand(unsigned char [28], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);

void bksmt_sha512t256_hkdf_expand(unsigned char [32], uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t);


#endif /* __BKSMT_HKDF_H__ */



