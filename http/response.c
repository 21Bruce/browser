#include "response.h"

#include "../net/conn.h"
#include "http.h"

#include <assert.h>
#include <stdio.h>

#define RBUFLEN 8000 

int
bksmt_http_res_recv(struct bksmt_http_res *res, struct bksmt_conn *conn)
{
    char rbuf[RBUFLEN];
    int pos, stat, r1f, n1f, r2f, n2f;
    size_t hlen;

    assert(res != NULL && conn != NULL);
    
    /* find delimeter between header and body */
    /* XXX: this is REALLY inefficient, should find a better way */
    r1f = n1f = r2f = n2f = 0;
    for(pos = 0, stat = bksmt_conn_mrecv(conn, rbuf, 1);
            stat == CONN_OK && pos < RBUFLEN; 
            pos++, stat = bksmt_conn_mrecv(conn, rbuf + pos, 1)) {

        if (rbuf[pos] == CR) { 
            if (!r1f) {
                r1f = 1;
                continue;
            }
            if (n1f && !r2f) {
                r2f = 1;
                continue;
            }
        } else if (rbuf[pos] == LF) {
            if (r1f && !n1f) {
                n1f = 1;
                continue;
            }
            if (r2f && !n2f) {
                n2f = 1;
                break;
            }
        }

        /* reset detector flags */
        r1f = r2f = n1f = n2f = 0;
    }

    /* if we don't have a header guard, err */
    if (!n2f)
        return HTTP_ERROR;
    
    hlen = pos + 1;

    fprintf(stderr, "%.*s", hlen, rbuf);

    return HTTP_OK;
}

