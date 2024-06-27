#ifndef __BKSMT_HTTP_HEADER_H__
#define __BKSMT_HTTP_HEADER_H__

#include "../lib/dict.h"
#include "../net/conn.h"
#include "http.h"

#include <stdlib.h>

/*
 * LUT for fast verb type to str conversion
 */
static struct bksmt_http_verb_lut_entry{
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
    int verbk; 
    char *fpath;
    int vminor;
    int vmajor;
    struct bksmt_dict *mfields;
};

struct bksmt_http_req {
    struct bksmt_http_req_header  header;
    unsigned char                *body;
    size_t                        blen;
}; 


int bksmt_http_req_init(char *, int, unsigned char *, size_t, int, struct bksmt_http_req **);

int bksmt_http_req_send(struct bksmt_http_req *, struct bksmt_conn *);

int bksmt_http_req_recv(struct bksmt_conn *, struct bksmt_http_req *); 

void bksmt_http_req_free(struct bksmt_http_req *);

/* init flags */

/* do not create mime fields */
#define BKSMT_HTTP_REQ_NOMIME 0x1

#endif
