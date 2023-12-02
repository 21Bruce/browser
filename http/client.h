#ifndef __BKSMT_HTTP_CLIENT_H__
#define __BKSMT_HTTP_CLIENT_H__

#include "request.h"
#include "response.h"
#include "../net/conn.h"

struct bksmt_http_history_req;

struct bksmt_http_history_res {
    struct bksmt_http_history_req *nxt;
    struct bksmt_http_history_req *prv;
    struct bksmt_http_res *res;
};

struct bksmt_http_history_req {
    struct bksmt_http_history_res *nxt;
    struct bksmt_http_history_res *prv;
    struct bksmt_http_req *req;
};

struct bksmt_http_interaction_history {
    struct bksmt_http_history_req *head;
    struct bksmt_http_history_res *tail;
};

struct bksmt_http_client {
    struct bksmt_http_interaction_history *hist;
    struct bksmt_conn *conn;
};

struct bksmt_http_client *bksmt_http_client_init(void);
