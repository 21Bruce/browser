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
        0x73, 0x93, 0x17, 0x2A
    };
    unsigned char key[24] = { 
        0x8E, 0x73, 0xB0, 0xF7, 
        0xDA, 0x0E, 0x64, 0x52, 
        0xC8, 0x10, 0xF3, 0x2B, 
        0x80, 0x90, 0x79, 0xE5, 
        0x62, 0xF8, 0xEA, 0xD2, 
        0x52, 0x2C, 0x6B, 0x7B
    };
    unsigned char out[16] = { 
        0xBD, 0x33, 0x4F, 0x1D, 
        0x6E, 0x45, 0xF2, 0x5F, 
        0xF7, 0x12, 0xA2, 0x14, 
        0x57, 0x1F, 0xA5, 0xCC
    };

    unsigned char store[16];

    bksmt_aes_192(in, key, store);

    if (memcmp(store, out, 16) != 0) {
        fprintf(stderr, "bksmt_aes_192 test FAILED\n");
        return 1;
    }

    bksmt_aes_inv_192(out, key, store);

    if (memcmp(store, in, 16) != 0) {
        fprintf(stderr, "bksmt_aes_inv_192 test FAILED\n");
        return 2;
    }

    fprintf(stderr, "all tests suceeded\n");
    return 0;
}
