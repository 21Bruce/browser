#include "hash.h"

#define DJB2 5381

unsigned long
djb2_hash(unsigned char *str)
{
    unsigned long hash = DJB2;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
