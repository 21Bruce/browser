#include "buf.h"
#include "xmalloc.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))

static int bksmt_buf_read_file(struct bksmt_buf *, unsigned char *, size_t);
static int bksmt_buf_read_mbuf(struct bksmt_buf *, unsigned char *, size_t);
static int bksmt_buf_write_file(struct bksmt_buf *, unsigned char *, size_t);
static int bksmt_buf_write_mbuf(struct bksmt_buf *, unsigned char *, size_t);


static int 
bksmt_buf_read_file(struct bksmt_buf *buf, unsigned char *mbuf,
        size_t buflen)
{

    size_t written = 0, min = MIN(buflen, buf->end - buf->start), nbytes; 
    
    lseek(buf->inf.fd, buf->start, SEEK_SET);

    written = 0;
    while((written < min) && (nbytes = read(buf->inf.fd, 
                    mbuf + written, min - written))) {
        if (nbytes <= 0)
            return -1;
        written += nbytes;
    }

    return min;
}

static int
bksmt_buf_read_mbuf(struct bksmt_buf *buf, unsigned char *mbuf,
        size_t buflen)
{
    size_t min = MIN(buflen, buf->end - buf->start); 
    memcpy(mbuf, buf->inf.mbuf, min);  
    return min;
}

static int 
bksmt_buf_write_file(struct bksmt_buf *buf, unsigned char *mbuf,
        size_t buflen)
{

    size_t written = 0, min = MIN(buflen, buf->end - buf->start), nbytes; 
 
    
    lseek(buf->inf.fd, buf->start, SEEK_SET);

    written = 0;
    while((written < min) && (nbytes = write(buf->inf.fd, 
                    mbuf + written, min - written))) {
        if (nbytes <= 0)
            return -1;
        written += nbytes;
    }

    return min;
}

static int
bksmt_buf_write_mbuf(struct bksmt_buf *buf, unsigned char *mbuf,
        size_t buflen)
{
    size_t min = MIN(buflen, buf->end - buf->start); 
    memcpy(buf->inf.mbuf, mbuf, min);  
    return min;
}

struct bksmt_buf *
bksmt_buf_init()
{
    return xmalloc(sizeof (struct bksmt_buf));
}

void
bksmt_buf_free(struct bksmt_buf *buf)
{
    assert(buf != NULL);
    free(buf);
}

int
bksmt_buf_read(struct bksmt_buf *buf, unsigned char *mbuf, size_t buflen)
{

    switch(buf->type) {
    case BUF_FILE:
        return bksmt_buf_read_file(buf, mbuf, buflen);
    case BUF_MSTAT:
    case BUF_MDYNA:
        return bksmt_buf_read_mbuf(buf, mbuf, buflen);
    default:
        return -1;
    }
}

int
bksmt_buf_write(struct bksmt_buf *buf, unsigned char *mbuf, size_t buflen)
{
    switch(buf->type) {
    case BUF_FILE:
        return bksmt_buf_write_file(buf, mbuf, buflen);
    case BUF_MSTAT:
    case BUF_MDYNA:
        return bksmt_buf_write_mbuf(buf, mbuf, buflen);
    default:
        return -1;
    }
}

struct bksmt_buf_chain *
bksmt_buf_chain_init()
{
    return xzalloc(sizeof (struct bksmt_buf_chain));
} 

void
bksmt_buf_chain_add(struct bksmt_buf_chain *chain, struct bksmt_buf *buf)
{
    struct bksmt_buf_chain *nelem;

    assert(chain != NULL);

    if (chain->buf == NULL) {
        chain->buf = buf;
        return;
    }

    nelem = bksmt_buf_chain_init();
    nelem->buf = buf;
    nelem->nxt = chain->nxt;  
    chain->nxt = nelem;
}

void 
bksmt_buf_chain_release(struct bksmt_buf_chain *chain)
{
    struct bksmt_buf_chain *c, *ctmp;

    assert(chain != NULL);

    for (c = chain; c != NULL; c = ctmp) {
        ctmp = c->nxt;
        free(c);
    }
}


