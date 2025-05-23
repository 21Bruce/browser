#include "../../crypto/aes.h"

#include <stdio.h>
#include <string.h>

int
main(void)
{
    unsigned char in[16] = { 
        0x6B, 0xC1, 0xBE, 0xE2,  
        0x2E, 0x40, 0x9F, 0x96,  
        0xE9, 0x3D, 0x7E, 0x11,  
        0x73, 0x93, 0x17, 0x2A,
    };
    unsigned char key[32] = { 
        0x60, 0x3D, 0xEB, 0x10,  
        0x15, 0xCA, 0x71, 0xBE,  
        0x2B, 0x73, 0xAE, 0xF0,  
        0x85, 0x7D, 0x77, 0x81,
        0x1F, 0x35, 0x2C, 0x07,  
        0x3B, 0x61, 0x08, 0xD7,  
        0x2D, 0x98, 0x10, 0xA3,  
        0x09, 0x14, 0xDF, 0xF4,
    };
    unsigned char out[16] = { 
        0xF3, 0xEE, 0xD1, 0xBD,  
        0xB5, 0xD2, 0xA0, 0x3C,  
        0x06, 0x4B, 0x5A, 0x7E,  
        0x3D, 0xB1, 0x81, 0xF8
    };

    unsigned char store[16];

    bksmt_aes_256(in, key, store);

    if (memcmp(store, out, 16) != 0) {
        fprintf(stderr, "bksmt_aes_256 test FAILED\n");
        return 1;
    }

    bksmt_aes_inv_256(out, key, store);

    if (memcmp(store, in, 16) != 0) {
        fprintf(stderr, "bksmt_aes_inv_256 test FAILED\n");
        return 2;
    }

    fprintf(stderr, "all tests suceeded\n");
    return 0;
}
