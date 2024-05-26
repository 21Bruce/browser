#include "../../crypto/aes.h"

#include <stdio.h>
#include <string.h>

int
main(void)
{
    unsigned char in[16] = { 
        0x32, 0x43, 0xf6, 0xa8, 
        0x88, 0x5a, 0x30, 0x8d, 
        0x31, 0x31, 0x98, 0xa2, 
        0xe0, 0x37, 0x07, 0x34 
    };
    unsigned char key[16] = { 
        0x2b, 0x7e, 0x15, 0x16, 
        0x28, 0xae, 0xd2, 0xa6, 
        0xab, 0xf7, 0x15, 0x88, 
        0x09, 0xcf, 0x4f, 0x3c 
    };
    unsigned char out[16] = { 
        0x39, 0x25, 0x84, 0x1d,
        0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97,
        0x19, 0x6a, 0x0b, 0x32,
    };

    unsigned char store[16];

    bksmt_aes_128(in, key, store);

    if (memcmp(store, out, 16) != 0) {
        fprintf(stderr, "bksmt_aes_128 test FAILED\n");
        return 1;
    }

    bksmt_aes_inv_128(out, key, store);

    if (memcmp(store, in, 16) != 0) {
        fprintf(stderr, "bksmt_aes_inv_128 test FAILED\n");
        return 2;
    }

    fprintf(stderr, "all tests suceeded\n");
    return 0;
}
