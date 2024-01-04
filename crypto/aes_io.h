#ifndef __BKSMT_CRYPTO_AES_IO_H__
#define __BKSMT_CRYPTO_AES_IO_H__

/* 
 * takes an input stream of 16 bytes and converts to a state
 * with the convention s[r,c] = i[r + 4c] 
 */
void bksmt_aes_in(unsigned char [16], unsigned char [16]);

/* 
 * takes a state and converts to an output stream of 16
 * bytes with the convention s[r,c] = o[r + 4c] 
 */
void bksmt_aes_out(unsigned char [16], unsigned char [16]);

#endif /* __BKSMT_CRYPTO_AES_IO_H__ */
