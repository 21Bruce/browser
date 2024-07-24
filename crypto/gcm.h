#ifndef __BKSMT_GCM_H__
#define __BKSMT_GCM_H__

#include <stdlib.h> 

/* supported tag lengths */
#define BKSMT_GCM_T128 16 
#define BKSMT_GCM_T120 15 
#define BKSMT_GCM_T112 14 
#define BKSMT_GCM_T104 13 
#define BKSMT_GCM_T96  12 

/* args: K, IV, P, Plen, AAD, AADLen, C, T, Tlen(one of the supported tag lengths */ 
void bksmt_gcm_aes_ae(unsigned char [16], unsigned char[12], unsigned char *, size_t, unsigned char *, size_t, unsigned char *, unsigned char *, size_t); 


#endif /* __BKSMT_GCM_H__ */
