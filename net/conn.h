#ifndef __BKSMT_CONN_H__
#define __BKSMT_CONN_H__

#include "buf.h"
#include <time.h>

struct bksmt_conn;

int bksmt_conn_open(char *, char *, char *, int, int, struct bksmt_conn **);

int bksmt_conn_send(struct bksmt_conn *, struct bksmt_buf *, size_t, time_t);

int bksmt_conn_recv(struct bksmt_conn *, struct bksmt_buf *, size_t, time_t);

int bksmt_conn_recv_chain(struct bksmt_conn *, struct bksmt_buf_chain *, size_t, time_t);

void bksmt_conn_close(struct bksmt_conn *);

/* connection type */
#define CONN_TCP 1
#define CONN_UDP 2
#define CONN_TLS 3

/* connection operation status */
#define CONN_OK     0
#define CONN_ERROR  1
#define CONN_AGAIN  2
#define CONN_BUSY   3
#define CONN_BAD    4

/* init flags */
#define CONN_LAZY 0x1
#define CONN_DNS  0x2
#define CONN_FLAG_SET(flags, flag) flags |= flag

#endif
