#ifndef __BKSMT_CRYPTO_AES_POLY_H__
#define __BKSMT_CRYPTO_AES_POLY_H__

/* FIPS-197 Section 4.2 */
unsigned char bksmt_bin_pmult(unsigned char, unsigned char);

/* FIPS-197 Section 4.3 */
void bksmt_byte_pmmult(unsigned char[4], unsigned char[4], unsigned char[4]);

#endif /* __BKSMT_CRYPTO_AES_POLY_H__ */
