#ifndef __BKSMT_HTTP_CLIENT_H__
#define __BKSMT_HTTP_CLIENT_H__

#include "../net/conn.h"
#include "../lib/buf.h"
#include "../lib/dict.h"
#include "../lib/dictcase.h"
#include "http.h"
#include "request.h"
#include "response.h"

struct bksmt_http_client {
    /* connection to send over */
    struct bksmt_conn     *conn;

    /* labelled dictionary map for cookies */
    struct bksmt_dictcase *cookiejar;

    /* curr domain, port, scheme, for connection management */
    char *cdomain;
    char *cport;
    char *cscheme;
};

struct bksmt_http_client *bksmt_http_client_init(void);

/* int ret is one of HTTP stat codes */
int bksmt_http_client_do(struct bksmt_http_client *, struct bksmt_http_req *, int, struct bksmt_http_res **);

void bksmt_http_client_free(struct bksmt_http_client *);

/* do flags */

/* do not store cookies */
#define HTTP_CLIENT_DO_NOCOOK 0x1

/* do not follow redirects */
#define HTTP_CLIENT_DO_NORED 0x2

/* do not upgrade */
#define HTTP_CLIENT_DO_NOUPG 0x2

#endif /* __BKSMT_HTTP_CLIENT_H__ */
