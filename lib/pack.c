#include "pack.h"

#include <stdint.h>
#include <stdio.h>

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

uint64_t
bksmt_packbe64(unsigned char src[8])
{
    int i; 
    uint64_t ret, tmp;

    ret = 0;
    for(i = 0; i < 8; i++) {
        tmp = src[i];
        ret |= tmp << (8 * (7-i));
    }

    return ret;
}

void 
bksmt_unpackbe64(uint64_t src, unsigned char out[8])
{
    int i;
    for (i = 0; i < 8; i++) {
        out[i] = src >> (8 * (7 - i));
    }
}

uint32_t
bksmt_packle32(unsigned char src[4])
{
    int i; 
    uint32_t ret;

    ret = 0;
    for(i = 0; i < 4; i++)
        ret |= src[i] << (8 * i);

    return ret;
}

void 
bksmt_unpackle32(uint32_t src, unsigned char out[4])
{
    int i;
    for (i = 0; i < 4; i++) {
        out[i] = src >> (8 * i);
    }
}

uint64_t
bksmt_packle64(unsigned char src[8])
{
    int i; 
    uint64_t ret, tmp;

    ret = 0;
    for(i = 0; i < 8; i++) {
        tmp = src[i];
        ret |= tmp << (8 * i);
    }

    return ret;
}

void 
bksmt_unpackle64(uint64_t src, unsigned char out[8])
{
    int i;
    for (i = 0; i < 8; i++) {
        out[i] = src >> (8 * i);
    }
}

