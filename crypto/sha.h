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


#endif /* __BKSMT_CRYPTO_SHA_H__ */
