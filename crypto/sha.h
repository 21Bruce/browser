#ifndef __BKSMT_CRYPTO_SHA_H__
#define __BKSMT_CRYPTO_SHA_H__


void sha1(unsigned char [20], unsigned char *, int);

void sha256(unsigned char[32], unsigned char *, int);

void sha224(unsigned char[24], unsigned char *, int);

void sha512(unsigned char[64], unsigned char *, int);

void sha384(unsigned char[48], unsigned char *, int);

void sha512t224(unsigned char[24], unsigned char *, int);

void sha512t256(unsigned char[32], unsigned char *, int);

#endif /* __BKSMT_CRYPTO_SHA_H__ */
