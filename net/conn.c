#include "conn.h"
#include "buf.h"
#include "../lib/xmalloc.h"

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <sys/errno.h>

#define CONN_ISLAZY(flags) (flags & CONN_LAZY )
#define CONN_ISDNS(flags) (flags & CONN_DNS  )

#define CONN_ISTCP(type)  (type & CONN_TCP)
#define CONN_ISUDP(type)  (type & CONN_UDP)
#define CONN_ISTLS(type)  (type & CONN_TLS)

#define MAXADDRLEN 256

struct bksmt_conn {
    /*
     * address and len
     */
    struct sockaddr_in addr;
    socklen_t addrlen;
    /*
     * lazy flag
     */
    int lazyf;
    /*
     * socket descriptor
     */
    int sd;
    /*
     * conn type
     */
    int type;
};

static int cstrtonum(char *);
static int pow(int, int);

static int
pow(int b, int e)
{
    int i = 0, exp = 1;

    while (i < e) {
        exp *= b;
        i++;
    }

    return exp;
}

static int
cstrtonum(char *str)
{
    int i;
    int sum;
    char c;

    i = strlen(str) - 1;
    sum = 0;
    while((c = *str++)) {
        sum += (c - '0') * pow(10,i);
        i--;
    }
    return sum;
}

int
bksmt_conn_open(char *addr, char *aport, char *port, int type, int flags, 
        struct bksmt_conn **ret)
{
    struct bksmt_conn *c;
    struct addrinfo hints, *paddr;
    int family, socktype, protocol;
    
    family = AF_INET;
    /*
     * For now, only support TLS and UDP
     */
    socktype = CONN_ISTCP(type) ? SOCK_STREAM
           : SOCK_DGRAM;
    protocol = CONN_ISTCP(type) ? IPPROTO_TCP 
           : IPPROTO_UDP;

    c = xzalloc(sizeof *c);    

    c->type = type;

    if (CONN_ISDNS(flags)) {
        memset(&hints, 0, sizeof hints);
        hints.ai_family = family;
        hints.ai_socktype = socktype; 
        if (getaddrinfo(addr, port, &hints, &paddr)) 
             goto abort;
        c->addrlen = paddr->ai_addrlen;
        memcpy(&(c->addr), paddr->ai_addr, sizeof (c->addr)); 
        freeaddrinfo(paddr);
    } else {
        c->addrlen = sizeof (c->addr); 
        c->addr.sin_family = htons(AF_INET);
        c->addr.sin_port = htons(cstrtonum(aport));
        if (inet_pton(family, addr, &(c->addr.sin_addr)) != 1) 
            goto abort;
   }

    c->sd = socket(family, socktype, protocol);
    if (c->sd == 0) 
        goto abort;

    c->lazyf = CONN_ISLAZY(flags);

    if (!c->lazyf && connect(c->sd, &(c->addr), c->addrlen))  
        goto abort;

    *ret = c;
    return CONN_OK;

abort:
    free(c);
    return CONN_ERROR;
}


