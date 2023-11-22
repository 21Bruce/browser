#ifndef __BKSMT_HEADER_H__
#define __BKSMT_HEADER_H__

#include "../lib/dict.h"
#include "http.h"

#include <stdlib.h>

/*
 * LUT for fast protocol type to str conversion
 */
struct bksmt_http_prot_lut_entry{
    char *prot;
    size_t len;
} bksmt_http_prot_lut[2] = {
#define HTTP_HTTP  0
    { "HTTP" , 4 },
#define HTTP_HTTPS 1
    { "HTTPS", 5 },
};

/*
 * LUT for fast verb type to str conversion
 */
struct bksmt_http_verb_lut_entry{
    char *verb;
    size_t len;
} bksmt_http_verb_lut[9] = {
#define HTTP_GET     0
    { "GET"    , 3 },
#define HTTP_HEAD    1
    { "HEAD"   , 4 },
#define HTTP_POST    2
    { "POST"   , 4 },
#define HTTP_PUT     3
    { "PUT"    , 3 },
#define HTTP_DELETE  4
    { "DELETE" , 6 },
#define HTTP_CONNECT 5
    { "CONNECT", 7 },
#define HTTP_TRACE   6
    { "TRACE"  , 5 },
#define HTTP_PATCH   7
    { "PATCH"  , 5 },
#define HTTP_OPTIONS 8
    { "OPTIONS", 7 },
};

struct bksmt_http_req_header {
    int protocol;
    int vminor;
    int vmajor;
    int verb; 
    struct bksmt_dict *fields;
};

struct bksmt_http_req {
    struct bksmt_http_req_header  header;
    struct bksmt_dict            *cookies;
    struct bksmt_buf             *body;
    struct bksmt_conn            *conn;
}; 

struct bksmt_http_req *bksmt_http_req_init();
int bksmt_http_req_parse(bksmt_buf *buf, struct bksmt_http_req **req);
int bksmt_http_req_send(struct bksmt_http_req *req);
void bksmt_http_req_close(struct bksmt_http_req *req);

/* http request status codes */
#define HTTP_OK     0
#define HTTP_ERROR  1
#define HTTP_BUSY   2

#endif
