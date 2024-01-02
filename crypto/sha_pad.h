#ifndef __BKSMT_CRYPTO_SHA_PAD_H__
#define __BKSMT_CRYPTO_SHA_PAD_H__

/* pad for SHA-{1,224,256}, length of output in bytes will be largest multiple of 64 closest to orig len */
unsigned char *bksmt_sha256_pad(unsigned char *, long);

/* returns length of a padded stream given initial length*/
long bksmt_sha256_pad_len(long);

/* pad for SHA-{384,512,512/224,512/256} length of output in bytes will be largest multiple of 128 closest to orig len */
unsigned char *bksmt_sha512_pad(unsigned char *, long);

/* returns length of a padded stream given initial length*/
long bksmt_sha512_pad_len(long);

#endif /* __BKSMT_CRYPTO_SHA_PAD_H__ */
