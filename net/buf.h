#ifndef __BKSMT_BUF_H__
#define __BKSMT_BUF_H__

struct bksmt_buf {
#define BUF_FILE 0x1
#define BUF_MMEM 0x2
#define BUF_MMAP 0x4
    int etype;
    int fd;
    unsigned char *buf;
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

struct bksmt_buf_chain {
    struct bksmt_buf *buf;
    struct bksmt_buf_chain *nxt;
};

struct bksmt_buf_chain *bksmt_buf_chain_init();

void bksmt_buf_chain_add(struct bksmt_buf_chain *, struct bksmt_buf *);

void bksmt_buf_chain_release(struct bksmt_buf_chain *);

#endif

