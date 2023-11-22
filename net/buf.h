#ifndef __BKSMT_BUF_H__
#define __BKSMT_BUF_H__

#include <stdlib.h>

struct bksmt_buf {
#define BUF_FILE 1
#define BUF_MMEM 2 
#define BUF_MMAP 3 
    int type;
    int fd;
    unsigned char *mbuf;
    /*
     * buffer is in [start, end-1]
     * positions of the inf
     */
    int start;
    int end; 
};

#define BKSMT_BUF_ISFILE(buf)  (buf)->etype &  BUF_FILE
#define BKSMT_BUF_SETFILE(buf) (buf)->etype &= BUF_FILE 

#define BKSMT_BUF_ISMMEM(buf)  (buf)->etype &  BUF_MMEM
#define BKSMT_BUF_SETMMEM(buf) (buf)->etype &= BUF_MMEM 

#define BKSMT_BUF_ISMMAP(buf)  (buf)->etype &  BUF_MMAP
#define BKSMT_BUF_SETMMAP(buf) (buf)->etype &= BUF_MMAP 

struct bksmt_buf *bksmt_buf_init();
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

