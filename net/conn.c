#include "conn.h"
#include "../lib/buf.h"
#include "../lib/xmalloc.h"
#include "../lib/strconv.h"

#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/errno.h>

#define CONN_ISDNS(flags)  (flags & CONN_DNS  )

#define CONN_ISTCP(type)  (type & CONN_TCP)
#define CONN_ISUDP(type)  (type & CONN_UDP)
#define CONN_ISTLS(type)  (type & CONN_TLS)

int
bksmt_conn_init(char *addr, char *aport, char *port, int type, int flags, 
        struct bksmt_conn **ret)
{
    struct bksmt_conn *c;
    struct addrinfo hints, *paddr;
    int family, socktype, protocol;
    
    /* init family, only support IPv4, NON-TLS for now */
    family = AF_INET;
    socktype = CONN_ISUDP(type) ? SOCK_DGRAM 
           : SOCK_STREAM;
    protocol = CONN_ISUDP(type) ? IPPROTO_UDP 
           : IPPROTO_TCP;

    /* allocate conn structure */
    c = xzalloc(sizeof *c);    
    c->type = type;

    /* decode address info, store for open() func */
    if (CONN_ISDNS(flags)) {
        /* 
         * if we have a DNS connection,
         * we have to perform a DNS
         * lookup on the addr field,
         * which should be a domain name
         */
        memset(&hints, 0, sizeof hints);
        hints.ai_family = family;
        hints.ai_socktype = socktype; 
        if (getaddrinfo(addr, port, &hints, &paddr)) 
             goto abort;
        c->addrlen = paddr->ai_addrlen;
        memcpy(&(c->addr), paddr->ai_addr, sizeof (c->addr)); 
        freeaddrinfo(paddr);
    } else {
        /*
         * otherwise, convert the aport
         * and addr to nbo and use those
         */
        c->addrlen = sizeof (c->addr); 
        c->addr.sin_family = htons(AF_INET);
        c->addr.sin_port = htons(cstrtoint(aport));
        if (inet_pton(family, addr, &(c->addr.sin_addr)) != 1) 
            goto abort;
   }

    *ret = c;
    return CONN_OK;

abort:
    free(c);
    return CONN_ERROR;
}

int
bksmt_conn_open(struct bksmt_conn *c)
{
    int family, socktype, protocol;
    assert(c != NULL);

    /* init family, only support IPv4, NON-TLS for now */
    family = AF_INET;
    socktype = CONN_ISUDP(c->type) ? SOCK_DGRAM 
           : SOCK_STREAM;
    protocol = CONN_ISUDP(c->type) ? IPPROTO_UDP 
           : IPPROTO_TCP;

   /*
    * init the socket descriptor
    * with vars above
    */
    c->sd = socket(family, socktype, protocol);
    if (c->sd == 0) 
        return CONN_ERROR;

    /* attempt connect() with saved info from init() */
    if (connect(c->sd, &(c->addr), c->addrlen))
        return CONN_ERROR;

    return CONN_OK;
}

int
bksmt_conn_send(struct bksmt_conn *c, struct bksmt_buf *b, 
        size_t nsend) {

    unsigned char *buf;
    size_t nbytes, written;

    assert(nsend <= b->end - b->start);

    /* init dynamic array buffer */
    buf = xmallocarray(nsend, sizeof *buf);

    /* read all of buffer into dynamic array*/
    nbytes = bksmt_buf_read(b, buf, nsend * sizeof *buf);
    if (nbytes != nsend)
        goto abort1;

    /* write buffer across connection */
    written = 0;
    while((written < nsend) && (nbytes = write(c->sd, 
                    buf + written, nsend - written))) {
        if (nbytes <= 0)
            goto abort1;
        written += nbytes;
    }

    free(buf);
    return CONN_OK;

abort1:
    free(buf);    
abort:
    return CONN_ERROR;
}

int
bksmt_conn_msend(struct bksmt_conn *c, unsigned char *buf, 
        size_t nsend) {

    size_t nbytes, written;

    /* write buffer across connection */
    written = 0;
    while((written < nsend) && (nbytes = write(c->sd, 
                    buf + written, nsend - written))) {
        if (nbytes <= 0)
            return CONN_ERROR;
        written += nbytes;
    }

    return CONN_OK;
}

int
bksmt_conn_mrecv(struct bksmt_conn *c, unsigned char *buf, 
        size_t nrecv) {

    size_t nbytes, written;

    written = 0;
    while((written < nrecv) && (nbytes = read(c->sd, 
                    buf + written, nrecv - written))) {
        if (nbytes <= 0)
            return CONN_ERROR;
        written += nbytes;
    }

    return CONN_OK;
}

int
bksmt_conn_recv(struct bksmt_conn *c, struct bksmt_buf *b, 
        size_t nrecv) {

    unsigned char *buf;
    size_t nbytes, written;

    assert(nrecv <= b->end - b->start);

    buf = xmallocarray(nrecv, sizeof *buf);

    written = 0;
    while((written < nrecv) && (nbytes = read(c->sd, 
                    buf + written, nrecv - written))) {
        if (nbytes <= 0)
            goto abort1;
        written += nbytes;
    }

    nbytes = bksmt_buf_write(b, buf, nrecv * sizeof *buf);
    if (nbytes != nrecv)
        goto abort1;

    free(buf);
    return CONN_OK;

abort1:
    free(buf);    
abort:
    return CONN_ERROR;
}

int 
bksmt_conn_recv_chain(struct bksmt_conn *c, 
        struct bksmt_buf_chain *ch, size_t nrecv)
{
    unsigned char *buf;
    size_t nbytes, written;

    buf = xmallocarray(nrecv, sizeof *buf);

    written = 0;
    while((written < nrecv) && (nbytes = read(c->sd, 
                    buf + written, nrecv - written))) {
        if (nbytes <= 0)
            goto abort1;
        written += nbytes;
    }

    for (; ch != NULL; ch = ch->nxt) {
        assert(nrecv <= ch->buf->end - ch->buf->start);
        nbytes = bksmt_buf_write(ch->buf, buf, nrecv * sizeof *buf);
        if (nbytes != nrecv)
            goto abort1;
    }
    free(buf);
    return CONN_OK;

abort1:
    free(buf);    
abort:
    return CONN_ERROR;
}

void
bksmt_conn_close(struct bksmt_conn *c)
{
    assert(c != NULL);
    close(c->sd);
}

void
bksmt_conn_free(struct bksmt_conn *c)
{
    assert(c != NULL);
    free(c);
}
