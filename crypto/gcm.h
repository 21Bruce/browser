#ifndef __BKSMT_GCM_H__
#define __BKSMT_GCM_H__

#include <stdlib.h> 

/* supported tag lengths */
#define BKSMT_GCM_T128 16 
#define BKSMT_GCM_T120 15 
#define BKSMT_GCM_T112 14 
#define BKSMT_GCM_T104 13 
#define BKSMT_GCM_T96  12 

/* decrypt statuses */

#define BKSMT_GCM_AD_OK   0x0
#define BKSMT_GCM_AD_FAIL 0x1

/* args: K, IV, P, Plen, AAD, AADLen, C, T, Tlen(one of the supported tag lengths) */ 
void bksmt_gcm_aes_128_ae(unsigned char [16], unsigned char[12], unsigned char *, size_t, unsigned char *, size_t, unsigned char *, unsigned char *, size_t); 

/* args: K, IV, C, Clen, AAD, AADLen, T, Tlen(one of the supported tag lengths), P */ 
int bksmt_gcm_aes_128_ad(unsigned char [16], unsigned char[12], unsigned char *, size_t, unsigned char *, size_t, unsigned char *, size_t, unsigned char *); 

#endif /* __BKSMT_GCM_H__ */
