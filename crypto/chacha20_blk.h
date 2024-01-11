#ifndef __BKSMT_CRYPTO_CHACHA20_BLK_H__
#define __BKSMT_CRYPTO_CHACHA20_BLK_H__

#include <stdint.h>

/* 
 * inputs: key, nonce, block #
 * outputs: ciphertext buffer 
 */
void bksmt_chacha20_blk(unsigned char [32], unsigned char [12], uint32_t, unsigned char [64]);

#endif /* __BKSMT_CRYPTO_CHACHA20_BLK_H__ */
