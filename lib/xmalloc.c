#include "xmalloc.h"

#include "math.h"

#include <stdlib.h>
#include <string.h>
#include <err.h>

#define ENOMEM -2
#define EOVERF -3

void *
xmalloc(size_t size) 
{
    unsigned char* ptr = malloc(size);
    if (ptr == NULL) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    }
    return ptr;
}

void *
xmallocarray(size_t nmemb, size_t size)
{
    unsigned long sizeb = nmemb * size;

    if(nmemb != 0 && sizeb/nmemb != size) {
        errx(EOVERF, "Overflow in allocating array");
        exit(EOVERF);
    }

    return xmalloc(nmemb * size);
} 

void *
xzallocarray(size_t nmemb, size_t size)
{
    void *ptr;
    unsigned long sizeb = nmemb * size;

    if(nmemb != 0 && sizeb/nmemb != size) {
        errx(EOVERF, "Overflow in allocating array");
        exit(EOVERF);
    }

    ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    }

    return ptr;
}

void *
xzalloc(size_t size)
{
    void *ptr;

    ptr = calloc(1, size);
    if (ptr == NULL) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    }

    return ptr;
}

void *
xrealloc(void *ptr, size_t csize, size_t nsize) 
{
    size_t msize;
    unsigned char* nptr;
   
    nptr = malloc(nsize);
    if (nptr == NULL) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    }

    msize = MIN(csize, nsize);

    memcpy(nptr, ptr, msize);
    free(ptr); 

    return nptr;
}

void *
xmdup(void *src, size_t size) 
{
    unsigned char* nptr;
   
    nptr = malloc(size);
    if (nptr == NULL) {
        errx(ENOMEM, "Ran out of memory");
        exit(ENOMEM);
    }

    memcpy(nptr, src, size);

    return nptr;
}


