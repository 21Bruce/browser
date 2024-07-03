#ifndef __BKSMT_BUFREAD_H__
#define __BKSMT_BUFREAD_H__

#include <pthread.h>

#define BKSMT_BUFREAD_SIZE    8192
#define BKSMT_BUFREAD_OK      0 
#define BKSMT_BUFREAD_ERR    -1
#define BKSMT_BUFREAD_EOF    -2
#define BKSMT_BUFREAD_ASYNC   1

struct bksmt_bufread {
    void                     *tap;
    unsigned char             buf[BKSMT_BUFREAD_SIZE];
    int                       pos;
    int                       size;
    int                       eof;
    int (*readtap)(void *, unsigned char *, int *);
    pthread_t                 *worker;
    pthread_mutex_t           *buflock;
};


struct bksmt_bufread *bksmt_bufread_init(void *, int (*)(void *, unsigned char *, int*));
int bksmt_bufread_read(struct bksmt_bufread *, unsigned char *, int, int *);
int bksmt_bufread_readall(struct bksmt_bufread *, unsigned char **);
void bksmt_bufread_free(struct bksmt_bufread *);


#endif /* __BKSMT_BUFREAD_H__ */
