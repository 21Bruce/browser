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

unsigned long
sdbm_hash(unsigned char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}
