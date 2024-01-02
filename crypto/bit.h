#ifndef __BKSMT_CRYPTO_BIT_H__
#define __BKSMT_CRYPTO_BIT_H__

#include <stdint.h>

/* make a big endian uint32 from a 4 byte stream.
 * Ex:  
 *         00 01 02 04
 * Input:  61 62 63 64 
 * Output: 0x61626364 
 */
uint32_t bksmt_packbe32(unsigned char[4]);

/* put a big endian uint32 in a 4 byte stream 
 * Ex:  
 * Input:  0x61626364 
 *         00 01 02 04
 * Output: 61 62 63 64 
 */
void bksmt_unpackbe32(uint32_t, unsigned char[4]);

#endif /* __BKSMT_CRYPTO_BIT_H__ */
