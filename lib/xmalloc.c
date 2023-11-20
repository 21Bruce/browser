#include "xmalloc.h"

#include <stdlib.h>
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
