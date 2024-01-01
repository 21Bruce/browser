#ifndef __BKSMT_CRYPTO_SHA_PAD_H__
#define __BKSMT_CRYPTO_SHA_PAD_H__

/* pad for SHA-{1,224,256}, length of output in bytes will be largest multiple of 64 closest to orig len */
unsigned char * sha256_pad(unsigned char *, long);


/* pad for SHA-{384,512,512/224,512/256} length of output in bytes will be largest multiple of 128 closest to orig len */
unsigned char * sha512_pad(unsigned char *, long);

#endif /* __BKSMT_CRYPTO_SHA_PAD_H__ */
