#ifndef __BKSMT_CRYPTO_SHA_H__
#define __BKSMT_CRYPTO_SHA_H__

/* each returns a byte array interpreted as a BE-int */

void bksmt_sha1(unsigned char[20], unsigned char *, int);

void bksmt_sha256(unsigned char[32], unsigned char *, int);

void bksmt_sha224(unsigned char[28], unsigned char *, int);

void bksmt_sha512(unsigned char[64], unsigned char *, int);

void bksmt_sha384(unsigned char[48], unsigned char *, int);

void bksmt_sha512t224(unsigned char[28], unsigned char *, int);

void bksmt_sha512t256(unsigned char[32], unsigned char *, int);

#endif /* __BKSMT_CRYPTO_SHA_H__ */
