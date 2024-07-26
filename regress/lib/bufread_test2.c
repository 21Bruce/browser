#include "../../lib/bufread.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BSIZE 16000

static int pos;

int
bufread(void *vbuf, unsigned char *out, int *size)
{
    unsigned char *buf;

    buf = (unsigned char *)vbuf;

    if (pos == BSIZE) {
        *size = 0;
        return BKSMT_BUFREAD_EOF;
    }
    if (pos + *size <= BSIZE) {
        memcpy(out, buf + pos, *size);
        pos += *size;
        return BKSMT_BUFREAD_OK;
    }
    memcpy(out, buf + pos, BSIZE-pos);
    *size = BSIZE-pos;
    pos = BSIZE;
    return BKSMT_BUFREAD_EOF;
}

int
main(void)
{
    struct bksmt_bufread *br;
    int i, size, stat;
    unsigned char buf[BSIZE], *out;

    for(i = 0; i < BSIZE; i++)
       buf[i] = i; 
    
    br = bksmt_bufread_init(buf, bufread, NULL);

    bksmt_bufread_readall(br, &out, &size);

    if (size != BSIZE)
        return -1;

    for(i = 0; i < BSIZE; i++)
       if (buf[i] != out[i]) {
           fprintf(stderr, "%u != %u\n", buf[i], out[i]);
           return i + 1;
       }
 

    free(out);
    bksmt_bufread_free(br);
    fprintf(stderr, "success\n");
    return 0; 
}
