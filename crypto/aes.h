#ifndef __BKSMT_CRYPTO_AES_H__
#define __BKSMT_CRYPTO_AES_H__

void bksmt_aes_128(unsigned char [16], unsigned char [16], unsigned char [16]);

void bksmt_aes_192(unsigned char [16], unsigned char [24], unsigned char [16]);

void bksmt_aes_256(unsigned char [16], unsigned char [32], unsigned char [16]);

void bksmt_aes_inv_128(unsigned char [16], unsigned char [16], unsigned char [16]);

void bksmt_aes_inv_192(unsigned char [16], unsigned char [24], unsigned char [16]);

void bksmt_aes_inv_256(unsigned char [16], unsigned char [32], unsigned char [16]);


#endif /* __BKSMT_CRYPTO_AES_H__ */
