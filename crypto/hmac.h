#ifndef __BKSMT_HMAC_H__
#define __BKSMT_HMAC_H__

#include <stdint.h>

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha256_hmac(unsigned char *, uint64_t ,  unsigned char [32], uint64_t, unsigned char [32]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha224_hmac(unsigned char *, uint64_t ,  unsigned char [32], uint64_t, unsigned char [28]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512_hmac(unsigned char *, uint64_t ,  unsigned char [64], uint64_t, unsigned char [64]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha384_hmac(unsigned char *, uint64_t ,  unsigned char [64], uint64_t, unsigned char [48]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512t224_hmac(unsigned char *, uint64_t ,  unsigned char [64], uint64_t, unsigned char [28]); 

/* Args: msg, msglen, key, keylen, output */
void bksmt_sha512t256_hmac(unsigned char *, uint64_t ,  unsigned char [64], uint64_t, unsigned char [32]); 

#endif /* __BKSMT_HMAC_H__ */
