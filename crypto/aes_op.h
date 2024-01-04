#ifndef __BKSMT_CRYPTO_AES_OP_H__
#define __BKSMT_CRYPTO_AES_OP_H__

/* each op is in-place */
void bksmt_aes_subbytes(unsigned char[16]);

void bksmt_aes_shiftrows(unsigned char[16]);

void bksmt_aes_mixcols(unsigned char[16]);

void bksmt_aes_addroundkey(unsigned char[16], unsigned char[4]);

#endif /* __BKSMT_CRYPTO_AES_OP_H__ */
