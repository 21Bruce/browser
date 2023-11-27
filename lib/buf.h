#ifndef __BKSMT_BUF_H__
#define __BKSMT_BUF_H__

#include <stdlib.h>
#include <assert.h>

struct bksmt_buf {
#define BUF_FILE  1
#define BUF_MSTAT 2 
#define BUF_MDYNA 3 
    int type;
    union bksmt_buf_inf {
        int fd;
        unsigned char *mbuf;
    } inf;
    /*
     * buffer is in [start, end-1]
     * positions of the inf
     */
    int start;
    int end; 
};

#define BKSMT_BUF_ATTACH(buf, typev, intv, startp, endp) \
    do {                                                 \
        assert(buf != NULL);                             \
        buf->type   = typev;                             \
        buf->inf    = (union bksmt_buf_inf) intv;        \
        buf->start  = startp;                            \
        buf->end    = endp;                              \
    } while (0)

struct bksmt_buf *bksmt_buf_init(void);

void              bksmt_buf_free(struct bksmt_buf *);

int               bksmt_buf_read(struct bksmt_buf *, unsigned char *, size_t);

int               bksmt_buf_write(struct bksmt_buf *, unsigned char *, size_t);

struct bksmt_buf_chain {
    struct bksmt_buf *buf;
    struct bksmt_buf_chain *nxt;
};

struct bksmt_buf_chain *bksmt_buf_chain_init();

void bksmt_buf_chain_add(struct bksmt_buf_chain *, struct bksmt_buf *);


void bksmt_buf_chain_release(struct bksmt_buf_chain *);

#endif

