#ifndef __BKSMT_CRYPTO_CHACHA20_BLK_IO_H__
#define __BKSMT_CRYPTO_CHACHA20_BLK_IO_H__

#include <stdint.h>

void bksmt_chacha20_blk_in(unsigned char [32], unsigned char [12], uint32_t, uint32_t [16]);

void bksmt_chacha20_blk_out(uint32_t [16], unsigned char [64]);

#endif /* __BKSMT_CRYPTO_CHACHA20_BLK_IO_H__ */
