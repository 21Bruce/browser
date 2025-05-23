#ifndef __BKSMT_CONN_H__
#define __BKSMT_CONN_H__

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

struct bksmt_conn;

int bksmt_conn_init(char *, char *, int, int, struct bksmt_conn **);

int bksmt_conn_open(struct bksmt_conn *);

#define bksmt_conn_is_open(conn) (conn->sd != -1)

int bksmt_conn_msend(struct bksmt_conn *, unsigned char *, size_t);

int bksmt_conn_mrecv(struct bksmt_conn *, unsigned char *, size_t);

void bksmt_conn_close(struct bksmt_conn *);

void bksmt_conn_free(struct bksmt_conn *);

/* connection type */
#define CONN_TCP 1
#define CONN_UDP 2
#define CONN_TLS 3

/* connection operation status */
#define CONN_OK     0
#define CONN_ERROR  1

/* init flags */
#define CONN_DNS  0x1
#define CONN_FLAG_SET(flags, flag) flags |= flag

/*
 * Implementation specific struct, DO NOT TOUCH
 */
struct bksmt_conn {
    /*
     * address and len
     */
    struct sockaddr_in addr;
    socklen_t addrlen;
    /*
     * socket descriptor
     */
    int sd;
    /*
     * conn type
     */
    int type;
};

#endif

