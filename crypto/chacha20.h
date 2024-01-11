#ifndef __BKSMT_CRYPTO_CHACHA20_H__
#define __BKSMT_CRYPTO_CHACHA20_H__

#include <stdint.h>

/* 
 * inputs: key, nonce, block #, plaintext, plaintext len
 * outputs: ciphertext
 */
void bksmt_chacha20(unsigned char [32], unsigned char [12], uint32_t, unsigned char *, int, unsigned char *);

#endif /* __BKSMT_CRYPTO_CHACHA20_BLK_H__ */
