#ifndef __BKSMT_HTTP_CLIENT_H__
#define __BKSMT_HTTP_CLIENT_H__

#include "../net/conn.h"
#include "../lib/buf.h"
#include "../lib/dict.h"
#include "../lib/dictcase.h"
#include "../lib/llkv.h"
#include "http.h"
#include "request.h"
#include "response.h"

struct bksmt_http_client {
    /* connection to send over */
    struct bksmt_conn     *conn;

    /* 
     * labelled labelled k-v store for cookies.
     * the abstract function of this data structure
     * it to take in a domain name and output
     * a dictcase, which represents the labelled
     * set of cookies(along with attributes) for 
     * this authority
     *
     */
    struct bksmt_llkv *cookiejar;

    /* curr domain, port, scheme, for connection management */
    char *cdomain;
    char *cport;
    char *cscheme;
};

struct bksmt_http_client *bksmt_http_client_init(void);

/* int ret is one of HTTP stat codes */
int bksmt_http_client_do(struct bksmt_http_client *, struct bksmt_http_req *, int, struct bksmt_http_res **);

/* char * is the authority, being "scheme://domain:port" */
struct bksmt_dictcase *bksmt_http_client_get_cookies(struct bksmt_http_client *, char *);

void bksmt_http_client_free(struct bksmt_http_client *);

/* do flags */

/* do not store cookies */
#define HTTP_CLIENT_DO_NOCOOK 0x1

/* do not follow redirects */
#define HTTP_CLIENT_DO_NORED 0x2

/* do not upgrade */
#define HTTP_CLIENT_DO_NOUPG 0x2

#endif /* __BKSMT_HTTP_CLIENT_H__ */
