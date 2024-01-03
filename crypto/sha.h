#ifndef __BKSMT_CRYPTO_SHA_H__
#define __BKSMT_CRYPTO_SHA_H__

/* each returns a byte array interpreted as a BE-int */

void bksmt_sha1(unsigned char *, int, unsigned char[20]);

void bksmt_sha256(unsigned char *, int, unsigned char[32]);
                                                         
void bksmt_sha224(unsigned char *, int, unsigned char[28]);
                                                         
void bksmt_sha512(unsigned char *, int, unsigned char[64]);
                                                         
void bksmt_sha384(unsigned char *, int, unsigned char[48]);

void bksmt_sha512t224(unsigned char *, int, unsigned char[28]);
                                                             
void bksmt_sha512t256(unsigned char *, int, unsigned char[32]);

#endif /* __BKSMT_CRYPTO_SHA_H__ */
