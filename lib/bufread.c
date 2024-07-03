#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "bufread.h"
#include "xmalloc.h"

static void flush_buf(struct bksmt_bufread *, int, unsigned char *);
static void fill_buf(struct bksmt_bufread *);

struct bksmt_bufread *
bksmt_bufread_init(void *tap, int (*readtap)(void *, unsigned char *, int*))
{
    struct bksmt_bufread *ret;

    ret = xmalloc(sizeof *ret);

    ret->tap = tap;
    ret->readtap = readtap;
    ret->pos = 0;
    ret->size = 0;
    ret->eof = 0;
    ret->buflock = xzalloc(sizeof *(ret->buflock));
    ret->worker = xzalloc(sizeof *(ret->worker));
    if (pthread_mutex_init(ret->buflock, NULL)) {
       free(ret->buflock); 
       free(ret->worker); 
       free(ret);
       fprintf(stderr, "ERR0\n");
       return NULL;
    }
    if (pthread_create(ret->worker, NULL, fill_buf, (void *) ret)) {
       free(ret->buflock); 
       free(ret->worker); 
       free(ret);
       fprintf(stderr, "ERR1\n");
       return NULL;
    }

    if (pthread_detach(*(ret->worker))) {
       free(ret->buflock); 
       free(ret->worker); 
       free(ret);
       fprintf(stderr, "ERR2\n");
       return NULL;
    }

    return ret;
}

static void
fill_buf(struct bksmt_bufread *br)
{
    while(1) {
        pthread_mutex_lock(br->buflock);
        pthread_mutex_unlock(br->buflock);
    }
}
int 
bksmt_bufread_read(struct bksmt_bufread *br, unsigned char *outbuf, int flag, int *size)
{
    int osize, extra, esize, stat;

    pthread_mutex_lock(br->buflock);

    if (br->size == 0 && br->eof) {
        *size = 0;
        pthread_mutex_unlock(br->buflock);
        return BKSMT_BUFREAD_EOF;
    }
        
    /* if we have enough in the buffer to back the call, then do it */
    if (size <= br->size) {
        flush_buf(br, size, outbuf);
        pthread_mutex_unlock(br->buflock);
        return BKSMT_BUFREAD_OK; 
    }

    /* if we reach here, br->size < size */
    osize = br->size;
    extra = size - br->size;

    /* flush the whole buffer */
    flush_buf(br, br->size, outbuf);

    /* if async flag is set, send out only what is in the buffer */
    if (flag & BKSMT_BUFREAD_ASYNC) {
        *size = osize;
        pthread_mutex_unlock(br->buflock);
        return BKSMT_BUFREAD_OK;
    }

    /* if we reach here, we need to source the rest of the request from the tap via the readtap func */
    esize = extra;
    stat = br->readtap(br->tap, outbuf + br->size, &esize);
    *size = osize + esize;

    /* if the tap encountered and EOF, we need to store this in the br */
    if (stat == BKSMT_BUFREAD_EOF) br->eof = 1;
    pthread_mutex_unlock(br->buflock);

    /* if there was an err or EOF, return that */
    return stat;

}

void
bksmt_bufread_free(struct bksmt_bufread *src)
{
    if (src->worker)
        free(src->worker);
    if (src->buflock)
        free(src->buflock);
    free(src);
}

/* read size bytes w/modular wrap around from pos in br->buf to out */
/* size must be <= br->size */
static void 
flush_buf(struct bksmt_bufread *br, int size, unsigned char *out)
{
    int diff;  

    /* CASE: no wraparound */
    if (br->pos + size <= BKSMT_BUFREAD_SIZE) {
        memcpy(out, br->buf + br->pos, size);
        br->pos = (br->pos + size) % BKSMT_BUFREAD_SIZE; 
        br->size -= size;
        return;
    }

    /* amt of wraparound */
    diff = (br->pos + size) % BKSMT_BUFREAD_SIZE;
    /* copy till end */
    memcpy(out, br->buf + br->pos, BKSMT_BUFREAD_SIZE - br->pos);
    /* copy wraparound */
    memcpy(out + BKSMT_BUFREAD_SIZE - br->pos, br->buf, diff);
    br->pos = (br->pos + size) % BKSMT_BUFREAD_SIZE; 
    br->size -= size;

}
