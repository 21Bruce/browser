#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>

#include "bufread.h"
#include "xmalloc.h"

static void flush_buf(struct bksmt_bufread *, int, unsigned char *);
static void fill_buf(struct bksmt_bufread *);
static void print_debug_info(struct bksmt_bufread *);

struct bksmt_bufread *
bksmt_bufread_init(void *tap, int (*readtap)(void *, unsigned char *, int*), void (*cleantap)(void*))
{
    struct bksmt_bufread *ret;

    ret = xmalloc(sizeof *ret);

    ret->tap = tap;
    ret->readtap = readtap;
    ret->cleantap = cleantap;
    ret->pos = 0;
    ret->size = 0;
    ret->workstat = BKSMT_BUFREAD_OK;
    ret->buflock = xzalloc(sizeof *(ret->buflock));
    ret->emptycond = xzalloc(sizeof *(ret->emptycond));
    ret->writethr = xzalloc(sizeof *(ret->writethr));
    if (pthread_mutex_init(ret->buflock, NULL)) goto fail; 
    if (pthread_cond_init(ret->emptycond, NULL)) goto fail; 
    if (pthread_create(ret->writethr, NULL, fill_buf, (void *) ret)) goto fail; 
    if (pthread_detach(*(ret->writethr))) goto fail; 
 
    return ret;

fail:
    free(ret->buflock); 
    free(ret->writethr); 
    free(ret);
    return NULL;
}

static void 
fill_buf(struct bksmt_bufread *br)
{
    int stat, esize, endpos;

    while(1) {
        pthread_mutex_lock(br->buflock);
        while(br->size == BKSMT_BUFREAD_SIZE && br->workstat == BKSMT_BUFREAD_OK)
            pthread_cond_wait(br->emptycond, br->buflock);

        /* if another thread read an EOF or error, give up lock and quit */
        if (br->workstat != BKSMT_BUFREAD_OK) {
            pthread_mutex_unlock(br->buflock);
            pthread_exit(0);
        }

        /* if we reach here, we can write to the buffer */

        endpos = (br->size + br->pos) % BKSMT_BUFREAD_SIZE;
        if (endpos > br->pos) {
            /* case 1: no wraparound */
            esize = BKSMT_BUFREAD_SIZE - endpos;
            stat = br->readtap(br->tap, br->buf + endpos, &esize);
        } else {
            /* case 2: wraparound */
            esize = br->pos - endpos;
            stat = br->readtap(br->tap, br->buf + endpos, &esize);
        }

        br->workstat = stat;
        br->size += esize;
        pthread_mutex_unlock(br->buflock);
    }
}

int 
bksmt_bufread_read(struct bksmt_bufread *br, unsigned char *outbuf, int flag, int *size)
{
    int osize, extra, esize, stat;

    pthread_mutex_lock(br->buflock);

    if (br->workstat == BKSMT_BUFREAD_ERR) {
        *size = 0;
        stat = BKSMT_BUFREAD_ERR;
        goto end;
    }

    if (br->size == 0 && br->workstat == BKSMT_BUFREAD_EOF) {
        *size = 0;
        stat = BKSMT_BUFREAD_EOF;
        goto end;
    }
        
    /* if we have enough in the buffer to back the call, then do it */
    if (*size <= br->size) {
        flush_buf(br, *size, outbuf);
        stat = BKSMT_BUFREAD_OK; 
        goto end;
    }


    /* if we reach here, br->size < size */
    osize = br->size;
    extra = *size - br->size;

    /* flush the whole buffer */
    flush_buf(br, br->size, outbuf);

    /* if async flag is set, send out only what is in the buffer */
    if (flag & BKSMT_BUFREAD_ASYNC) {
        *size = osize;
        stat = BKSMT_BUFREAD_OK;
        goto end;
    }

    /* if we reach here, we need to source the rest of the request from the tap via the readtap func */
    esize = extra;
    stat = br->readtap(br->tap, outbuf + osize, &esize);
    *size = osize + esize;

    /* if the tap encountered an EOF or ERR, we need to store this in the br */
    br->workstat = stat;

    /* if there was an ERR or EOF, return that */
end:
    pthread_mutex_unlock(br->buflock);
    pthread_cond_signal(br->emptycond);
    return stat;

}

void
bksmt_bufread_free(struct bksmt_bufread *src)
{
    /* force worker to exit */
    pthread_mutex_lock(src->buflock);
    src->workstat = BKSMT_BUFREAD_EOF;
    pthread_mutex_unlock(src->buflock);
    pthread_cond_signal(src->emptycond);
    pthread_join(*(src->writethr), NULL);

    /* run cleanup routine if we have one */
    if (src->cleantap) src->cleantap(src->tap);

    free(src->writethr);
    pthread_mutex_destroy(src->buflock);
    pthread_cond_destroy(src->emptycond);
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
    br->pos = diff; 
    br->size -= size;
}

static void 
print_debug_info(struct bksmt_bufread *br)
{
    int i;
    fprintf(stderr, "Buffer Pos: %d\n", br->pos); 
    fprintf(stderr, "Buffer Size: %d\n", br->size); 
}

