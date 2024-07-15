#include "../../lib/bufread.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define BSIZE 16000

static int pos;

int
bufread(unsigned char *buf, unsigned char *out, int *size)
{
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
    int i, j, size, osize, stat, ef;
    unsigned char buf[BSIZE], out[BSIZE/400];
    pos = 0;

    for(i = 0; i < BSIZE; i++)
       buf[i] = i; 
    
    br = bksmt_bufread_init(buf, bufread, NULL);

    osize = 0;
    size = BSIZE/400;
    ef = 0;
    stat = BKSMT_BUFREAD_OK;
    while(osize != BSIZE) {
        size = BSIZE/400;
        stat = bksmt_bufread_read(br, out, 0, &size);
        fprintf(stderr, "read from main: size: %d, osize: %d\n", size, osize);
        if (size == 0 && stat == BKSMT_BUFREAD_EOF)
            return -1;
        for(i = 0; i < size; i++)
            if (out[i] != buf[osize + i])
                ef = osize + i + 1;
        if (ef) {
            for(i = 0; i < size; i++)
                fprintf(stderr, "%d %d\n", out[i], buf[osize + i]);
            return ef;
        }
        osize += size;
    }


    bksmt_bufread_free(br);
    fprintf(stderr, "success\n");
    return 0; 
}
