#include "../../lib/pack.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

int
main(void) 
{
    // testing 32-bit big-endian packing
    unsigned char belbase[4] = { 0x0, 0x1, 0x2, 0x3 };
    unsigned char lres[4] = { 0x0, 0x0, 0x0, 0x0 };
    uint32_t ilres;

    if ((ilres = bksmt_packbe32(belbase)) != 0x00010203) {
        fprintf(stderr, "bksmt_packbe32 test FAILED\n");
        return 1;
    }

    bksmt_unpackbe32(ilres, lres);

    if (memcmp(lres, belbase, 4) != 0) {
        fprintf(stderr, "bksmt_unpackbe32 test FAILED\n");
        return 2; 
    }

    // testing 32-bit little-endian packing
    unsigned char lelbase[4] = { 0x3, 0x2, 0x1, 0x0 };

    if ((ilres = bksmt_packle32(lelbase)) != 0x00010203) {
        fprintf(stderr, "bksmt_packle32 test FAILED\n");
        return 3;
    }

    bksmt_unpackle32(ilres, lres);

    if (memcmp(lres, lelbase, 4) != 0) {
        fprintf(stderr, "bksmt_unpackle32 test FAILED\n");
        return 4; 
    }

    // testing 64-bit big-endian packing
    unsigned char bellbase[8] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 };
    unsigned char llres[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    uint64_t illres;

    if ((illres = bksmt_packbe64(bellbase)) != 0x0001020304050607) {
        fprintf(stderr, "bksmt_packbe64 test FAILED\n");
        return 5;
    }

    bksmt_unpackbe64(illres, llres);

    if (memcmp(llres, bellbase, 8) != 0) {
        fprintf(stderr, "bksmt_unpackbe64 test FAILED\n");
        return 6; 
    }

    // testing 64-bit little-endian packing
    unsigned char lellbase[8] = { 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0 };

    if ((illres = bksmt_packle64(lellbase)) != 0x0001020304050607) {
        fprintf(stderr, "bksmt_packle64 test FAILED\n");
        return 7;
    }

    bksmt_unpackle64(illres, llres);

    if (memcmp(llres, lellbase, 8) != 0) {
        fprintf(stderr, "bksmt_unpackle64 test FAILED\n");
        return 8; 
    }

    fprintf(stderr, "all tests succeeded\n");
    return 0;
}
