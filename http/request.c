#include "request.h"

#include "../lib/dict.h"
#include "../lib/buf.h"
#include "../lib/xstring.h"
#include "../lib/xmalloc.h"
#include "http.h"
#include "uri.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int
bksmt_http_req_init(char *uri, int verbk, struct bksmt_buf *body, int flags, struct bksmt_http_req **rreq)
{
    struct bksmt_uri *ustct;
    struct bksmt_http_req *req;

    assert(uri != NULL);
    assert(0 <= verbk && verbk <= 8);

    if (bksmt_uri_parse(uri, &ustct) != HTTP_OK) {
        bksmt_uri_free(ustct);
        return HTTP_ERROR;
    }
   
    /* alloc and fill a standard request */
    req = xmalloc(sizeof *req);
    req->header.verbk = verbk;
    bksmt_uri_build(ustct, &(req->header.fpath), 0);
    /* XXX: default to 1.1 for now */
    req->header.vminor = 1;
    req->header.vmajor = 1;
    req->header.mfields = bksmt_dict_init();
    req->body = body;

    /* if we don't want mime, quit here */
    if (BKSMT_HTTP_REQ_NOMIME & flags) {
        bksmt_uri_free(ustct);
        *rreq = req;
        return HTTP_OK;
    }


    /* set mime headers */
    bksmt_dict_set(req->header.mfields, "Host", ustct->dn);
    if (ustct->fpath == NULL)
        bksmt_dict_set(req->header.mfields, "Filename", "/");
    else 
        bksmt_dict_set(req->header.mfields, "Filename", ustct->fpath);
    bksmt_dict_set(req->header.mfields, "Scheme", bksmt_http_prot_lut[ustct->protocolk].prot);

    /* free residual uri struct and return */
    bksmt_uri_free(ustct);
    *rreq = req;
    return HTTP_OK;
}

int
bksmt_http_req_send(struct bksmt_http_req *req, struct bksmt_conn *conn)
{
    unsigned char *bytes;
    char sp, sl, *dcrlf, *flend, *hpair;
    size_t pathlen, flendlen, hpairlen;
    struct bksmt_http_verb_lut_entry vk;
    struct bksmt_dict_elem *e;
    int stat;

    assert(req != NULL);

    /* special chars */
    sp = ' ';
    sl = '/'; 
    dcrlf = "\r\n";

    vk = bksmt_http_verb_lut[req->header.verbk];

    /* send verb and a space */
    stat = bksmt_conn_msend(conn, vk.verb, vk.len);
    if (stat == CONN_ERROR)
        return HTTP_ERROR;

    stat = bksmt_conn_msend(conn, &sp, 1);
    if (stat == CONN_ERROR)
        return HTTP_ERROR;
 
    pathlen = strlen(req->header.fpath);

    /* send file path and a space */
    stat = bksmt_conn_msend(conn, req->header.fpath, pathlen);
    if (stat == CONN_ERROR)
        return HTTP_ERROR;

    stat = bksmt_conn_msend(conn, &sp, 1);
    if (stat == CONN_ERROR)
        return HTTP_ERROR;

    /* construct end of first line */
    flendlen = xasprintf(&flend, "HTTP/%d.%d\r\n", 
            req->header.vmajor, req->header.vminor);
    
    /* send end of first line */
    stat = bksmt_conn_msend(conn, flend, flendlen);

    /* free resources to construct first line */
    free(flend);

    /* check status of connection */
    if (stat == CONN_ERROR) {
        return HTTP_ERROR;
    }

    /* send mime fields */
    if (req->header.mfields) {
        BKSMT_DICT_FOREACH(req->header.mfields, e) {
            hpairlen = xasprintf(&hpair, "%s: %s\r\n", e->key, e->val);
            stat = bksmt_conn_msend(conn, hpair, hpairlen);
            free(hpair);
            if (stat == CONN_ERROR) 
                return HTTP_ERROR;
        }
    }

    /* send header terminator */
    stat = bksmt_conn_msend(conn, dcrlf, 2);
    if (stat == CONN_ERROR) 
        return HTTP_ERROR;
 
    /* send body */
    if (req->body) {
        stat = bksmt_conn_send(conn, req->body, req->body->end - req->body->start);
        if (stat == CONN_ERROR)
            return HTTP_ERROR;
    }

    return HTTP_OK;
}

void
bksmt_http_req_clear(struct bksmt_http_req *req)
{
    assert(req != NULL);

    if (req->header.fpath)
        free(req->header.fpath);

    if (req->header.mfields) 
        bksmt_dict_free(req->header.mfields);

    if (req->body)
        switch(req->body->type) {
        case BUF_MDYNA:
            free(req->body->inf.mbuf);
            return;
        case BUF_FILE:
            close(req->body->inf.fd);
            return;
        }
}

void
bksmt_http_req_free(struct bksmt_http_req *req)
{
    assert(req != NULL);

    bksmt_http_req_clear(req);
    free(req);
}
