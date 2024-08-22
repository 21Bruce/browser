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

/* SHA api methods allowing for padding around input message */

/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha1_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[20]);

/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha256_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[32]);
                                                         
/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha224_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[28]);

/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha512_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[64]);
                                                         
/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha384_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[48]);

/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha512t224_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[28]);
                                                         
/* Args: front pad, len, msg, len, back pad, len, output */
void bksmt_sha512t256_multi(unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char *, uint64_t, unsigned char[32]);


#endif /* __BKSMT_CRYPTO_SHA_H__ */
