#include "pack.h"

#include <stdint.h>

uint32_t
bksmt_packbe32(unsigned char src[4])
{
    int i; 
    uint32_t ret;

    ret = 0;
    for(i = 0; i < 4; i++)
        ret |= src[i] << (8 * (3-i));

    return ret;
}

void 
bksmt_unpackbe32(uint32_t src, unsigned char out[4])
{
    int i;
    for (i = 0; i < 4; i++) {
        out[i] = src >> (8 * (3 - i));
    }
}

