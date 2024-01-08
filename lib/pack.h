#ifndef __BKSMT_LIB_PACK_H__
#define __BKSMT_LIB_PACK_H__

#include <stdint.h>

/* make a big endian uint32 from a 4 byte stream.
 * Ex:  
 *         00 01 02 03
 * Input:  61 62 63 64 
 * Output: 0x61626364 
 */
uint32_t bksmt_packbe32(unsigned char[4]);

/* put a big endian uint32 in a 4 byte stream 
 * Ex:  
 * Input:  0x61626364 
 *         00 01 02 03
 * Output: 61 62 63 64 
 */
void bksmt_unpackbe32(uint32_t, unsigned char[4]);

/* make a big endian uint64 from an 8 byte stream.
 * Ex:  
 *         00 01 02 03 04 05 06 07
 * Input:  61 62 63 64 65 66 67 68
 * Output: 0x6162636465666768 
 */
uint64_t bksmt_packbe64(unsigned char[8]);

/* put a big endian uint64 in a 4 byte stream 
 * Ex:  
 * Input:  0x6162636465666768 
 *         00 01 02 03 04 05 06 07
 * Output: 61 62 63 64 65 66 67 68
 */
void bksmt_unpackbe64(uint64_t, unsigned char[8]);

/* make a little endian uint32 from a 4 byte stream.
 * Ex:  
 *         00 01 02 03
 * Input:  61 62 63 64 
 * Output: 0x64636261
 */
uint32_t bksmt_packle32(unsigned char[4]);

/* put a little endian uint32 in a 4 byte stream 
 * Ex:  
 * Input:  0x61626364 
 *         00 01 02 03
 * Output: 64 63 62 61 
 */
void bksmt_unpackle32(uint32_t, unsigned char[4]);

/* make a little endian uint64 from an 8 byte stream.
 * Ex:  
 *         00 01 02 03 04 05 06 07
 * Input:  61 62 63 64 65 66 67 68
 * Output: 0x6867666564636261 
 */
uint64_t bksmt_packle64(unsigned char[8]);

/* put a little endian uint32 in a 4 byte stream 
 * Ex:  
 * Input:  0x6162636465666768 
 *         00 01 02 03 04 05 06 07
 * Output: 68 67 66 65 64 63 62 61
 */
void bksmt_unpackle64(uint64_t, unsigned char[8]);

#endif /* __BKSMT_LIB_PACK_H__ */
