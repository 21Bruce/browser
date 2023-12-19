#ifndef __BKSMT_HTTP_HISTORY_H__
#define __BKSMT_HTTP_HISTORY_H__

#include <stdlib.h>

#include "request.h"
#include "response.h"
#include "../net/conn.h"

struct bksmt_http_history_node {
#define HIST_TYPE_REQ 1
#define HIST_TYPE_RES 2
    int mtype;

    union {
        struct bksmt_http_history_req *req;
        struct bksmt_http_history_res *res;
    } msg;

    struct bksmt_http_history_node *nxt;
    struct bksmt_http_history_node *prv;
}

struct bksmt_http_history {
    struct bksmt_http_history_node *head;
    struct bksmt_http_history_node *tail;
    size_t len;
};

struct bksmt_http_history *bksmt_http_history_init(void);

struct bksmt_http_history *bksmt_http_history_dup(struct bksmt_http_history *);

struct bksmt_http_history *bksmt_http_history_search_key(struct bksmt_http_history *, char *);

struct bksmt_http_history *bksmt_http_history_search_val(struct bksmt_http_history *, char *);

void bksmt_http_history_free(struct bksmt_http_history *);

#endif /* __BKSMT_HTTP_HISTORY_H__ */

