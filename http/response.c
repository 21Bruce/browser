#include "response.h"

#include "../net/conn.h"
#include "../lib/xstring.h"
#include "../lib/strconv.h"
#include "../lib/xmalloc.h"
#include "mime.h"
#include "http.h"
#include "cookie.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define RBUFLEN 8000 

/* locals for parsing */
static int parse_header(struct bksmt_http_res *, unsigned char *, size_t); 

static char *adv_ws(char *, char *);

static int inttostatk(int);

/* ok */
#define HTTP_RES_PARSE_OK    0
/* error */
#define HTTP_RES_PARSE_ERROR 1

static char *
adv_ws(char *src, char *end)
{
    while (src < end && *src == ' ')
        src++;
    return src;
}

static int
inttostatk(int statc)
{
#define intokstatk(x) HTTP_ ##x
#define statconvcase(x) case x: return intokstatk(x) 
    switch(statc) {
    statconvcase(100);
    statconvcase(101);
    statconvcase(102);
    statconvcase(103);
    statconvcase(200);
    statconvcase(201);
    statconvcase(202);
    statconvcase(203);
    statconvcase(204);
    statconvcase(205);
    statconvcase(206);
    statconvcase(207);
    statconvcase(208);
    statconvcase(226);
    statconvcase(300);
    statconvcase(301);
    statconvcase(302);
    statconvcase(303);
    statconvcase(304);
    statconvcase(305);
    statconvcase(306);
    statconvcase(307);
    statconvcase(400);
    statconvcase(401);
    statconvcase(402);
    statconvcase(403);
    statconvcase(404);
    statconvcase(405);
    statconvcase(406);
    statconvcase(407);
    statconvcase(408);
    statconvcase(409);
    statconvcase(410);
    statconvcase(411);
    statconvcase(412);
    statconvcase(413);
    statconvcase(414);
    statconvcase(415);
    statconvcase(416);
    statconvcase(417);
    statconvcase(418);
    statconvcase(421);
    statconvcase(422);
    statconvcase(423);
    statconvcase(424);
    statconvcase(425);
    statconvcase(426);
    statconvcase(428);
    statconvcase(429);
    statconvcase(431);
    statconvcase(451);
    statconvcase(500);
    statconvcase(501);
    statconvcase(502);
    statconvcase(503);
    statconvcase(504);
    statconvcase(505);
    statconvcase(506);
    statconvcase(507);
    statconvcase(508);
    statconvcase(510);
    statconvcase(511);
    default: return -1;
    } 
}


static int 
parse_header(struct bksmt_http_res *res, unsigned char *buf, size_t len) 
{
    char *cptr, *end, *fdot, *nws, *ncrlf, *fdcrlf, *ncol, *tmpk, *tmpv;
    struct bksmt_map *cookie;
    int stat;

    cptr = buf;
    end = buf + len;

    if (strncasecmp(cptr, "HTTP/", 5) != 0)
        return HTTP_RES_PARSE_ERROR;

    cptr += 5;

    if (cptr >= end)
        return HTTP_RES_PARSE_ERROR;

    fdot = strchr(cptr, '.');
    if (fdot == NULL)
        return HTTP_RES_PARSE_ERROR;

    res->header.vmajor = csubstrtoint(cptr, fdot);

    cptr = fdot + 1;
    if (cptr >= end)
        return HTTP_RES_PARSE_ERROR;

    nws = strchr(cptr, ' ');
    if (nws == NULL)
        return HTTP_RES_PARSE_ERROR;

    res->header.vminor = csubstrtoint(cptr, nws);

    cptr = adv_ws(nws, end);
    if (cptr >= end)
        return HTTP_RES_PARSE_ERROR;

    nws = strchr(cptr, ' ');
    if (nws == NULL)
        return HTTP_RES_PARSE_ERROR;

    res->header.statk = inttostatk(csubstrtoint(cptr, nws));
    if (res->header.statk == -1)
        return HTTP_RES_PARSE_ERROR;

    ncrlf = strstr(cptr, "\r\n");
    if (ncrlf == NULL)
        return HTTP_RES_PARSE_ERROR;

    cptr = ncrlf + 2;

    /* since we are 0 terminated at last \n */
    fdcrlf = strstr(cptr, "\r\n\r");
    if (fdcrlf == NULL)
        return HTTP_RES_PARSE_ERROR;

    /* init header and cookie data structures if they aren't */
    if (res->header.mfields == NULL)
        res->header.mfields = bksmt_dict_init();

    if (res->header.cookies == NULL)
        res->header.cookies = bksmt_dictcase_init();

    /* loop for each header, and at the start of loop, find next CRLF */
    /* the next CRLF is the position where the current header ends */
    while((ncrlf = strstr(cptr, "\r\n"))) {
        /* find delim btwn key and val */
        ncol = strchr(cptr, ':');

        /* if we moved cursor passed the current CRLF, err */
        if (ncol == NULL || ncol > ncrlf)
            goto kerror;

        /* copy key into dynamic buffer */
        xasprintf(&tmpk, "%.*s", ncol - cptr, cptr);

        /* move cursor past whitespace */
        cptr = adv_ws(ncol + 1, end);

        /* if cursor ends at CRLF, no value, so err */
        if (cptr == ncrlf) 
            goto verror;

        /* copy value into dynamic buffer */
        xasprintf(&tmpv, "%.*s", ncrlf - cptr, cptr);

        /* transform key into canonical mime form */
        bksmt_cstrmime(tmpk);

        /* check if this is 'Set-Cookie' special case. 
         * multiple 'Set-Cookie' headers can be attached
         * to one response message, and they CAN NOT 
         * be collapsed into one header k-v pair 
         * according to RFC 7230.
         */
        if (strcmp("Set-Cookie", tmpk) == 0) {
            stat = parse_cookie(tmpv, &cookie);
            if (stat != COOKIE_OK)
                goto cerror;
            /* add cookie to cookie dictcase labelled under 'Name' */ 
            bksmt_dictcase_set(res->header.cookies, 
                    bksmt_dict_get(cookie, "Name"), cookie);
        } else {
            bksmt_dict_set(res->header.mfields, tmpk, tmpv);
        }

        /* release dynam buffers */
        free(tmpk);
        free(tmpv);

        /* move cursor to start of next header */
        cptr = ncrlf + 2;
    }
    return HTTP_RES_PARSE_OK;

cerror:
    /* cookie parsing error, we already have value and key */
    free(tmpv);
verror:
    /* value error, we already have key */
    free(tmpk);
kerror:
    /* key error, we only have initted resources */
    bksmt_dict_free(res->header.mfields);
    bksmt_dict_free(res->header.cookies);
    res->header.mfields = NULL;
    res->header.cookies = NULL;
    return HTTP_RES_PARSE_ERROR;
}

int
bksmt_http_res_recv(struct bksmt_http_res *res, struct bksmt_conn *conn)
{
    char rbuf[RBUFLEN], chlenbuf[sizeof(size_t)];
    int pos, stat, r1f, n1f, r2f, n2f, endf;
    size_t hlen, chlen, blen;
    char *lenstr; 
    unsigned char *bufbck, *tmpbuf;

    assert(res != NULL && conn != NULL);

    /* init res body and bufbck */
    res->body = NULL;
    bufbck = "";
    blen = 0;

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

    /* turn into cstr for header parsing */
    hlen = pos; 
    rbuf[pos] = 0;

   /* parse headers */

    stat = parse_header(res, rbuf, hlen);
    if (stat == HTTP_RES_PARSE_ERROR) 
        return HTTP_ERROR;


    /* parse body */

    /* if there are no mime fields, we have no body */
    if (res->header.mfields == NULL)
        return HTTP_OK;

    /* if there is a content length, we read it all in */
    if((lenstr = bksmt_dict_get(res->header.mfields, "Content-Length"))) {
        chlen = cstrtoint(lenstr);
        bufbck = xmallocarray(chlen, sizeof *bufbck);
        stat = bksmt_conn_mrecv(conn, bufbck, chlen);
        if (stat == CONN_ERROR) {
            free(bufbck);
            return HTTP_ERROR;
        }
        res->body = bufbck;
        res->blen = chlen;
        return HTTP_OK;
    }

    /* if there is a chunked transfer encoding, parse it and read it in */
    if (strcasecmp(bksmt_dict_get(res->header.mfields, "Transfer-Encoding"), "chunked") == 0) {
        for(; ;) {
            for(pos = 0, stat = bksmt_conn_mrecv(conn, chlenbuf, 1);
                    chlenbuf[pos] != '\r'; 
                    pos++, stat = bksmt_conn_mrecv(conn, chlenbuf + pos, 1)) {
                if (stat != CONN_OK || pos >= sizeof(size_t)) 
                    goto error;
            }
            stat = bksmt_conn_mrecv(conn, rbuf, 1);
            if (stat == CONN_ERROR) 
                goto error;
            chlen = hexcsubstrtoint(chlenbuf, chlenbuf + pos);
            if (chlen == 0) {
                stat = bksmt_conn_mrecv(conn, rbuf, 2);
                if (stat == CONN_ERROR) 
                    goto error;
                /* if there were no chunks, do not init a buf */
                if (blen == 0)
                    return HTTP_OK;
                res->body = bufbck;
                res->blen = blen;
                return HTTP_OK;
            }
            tmpbuf = xmallocarray(chlen, sizeof *tmpbuf);
            stat = bksmt_conn_mrecv(conn, tmpbuf, chlen);
            if (stat == CONN_ERROR) 
                goto error1;
            blen = xasprintf(&bufbck, "%s%.*s", bufbck, chlen, tmpbuf);
            free(tmpbuf);
            stat = bksmt_conn_mrecv(conn, rbuf, 2);
            if (stat == CONN_ERROR) 
                goto error;
        }
error1:
        free(tmpbuf);
error:
        if (bufbck != NULL)
            free(bufbck);
        return HTTP_ERROR;
    }

    return HTTP_OK;
}


int
bksmt_http_res_send(struct bksmt_http_res *res, struct bksmt_conn *conn)
{
    char sp, sl, *dcrlf, *tmpstr;
    size_t tmpstrlen;
    struct bksmt_http_status_lut_entry sk;
    struct bksmt_map_elem *e;
    struct bksmt_dictcase_elem *de;
    int stat;

    assert(res != NULL);

    /* special chars */
    sp = ' ';
    sl = '/'; 
    dcrlf = "\r\n";

    /* construct first line using lut key */
    sk = bksmt_http_status_lut[res->header.statk];
    tmpstrlen = xasprintf(&tmpstr, "HTTP/%d.%d %.*s %.*s\r\n", 
            res->header.vmajor, res->header.vminor, 3, sk.code, sk.msglen, sk.stat_msg);

    /* send first line */
    stat = bksmt_conn_msend(conn, tmpstr, tmpstrlen);
    free(tmpstr);

    /* If conn failed, err */
    if (stat == CONN_ERROR)
        return HTTP_ERROR;

    /* send mime fields */
    if (res->header.mfields) {
        BKSMT_DICT_FOREACH(res->header.mfields, e) {
            tmpstrlen = xasprintf(&tmpstr, "%s: %s\r\n", e->key, e->val);
            stat = bksmt_conn_msend(conn, tmpstr, tmpstrlen);
            free(tmpstr);
            if (stat == CONN_ERROR) 
                return HTTP_ERROR;
        }
    }

    /* send cookies */
    if (res->header.cookies) {
        BKSMT_DICTCASE_FOREACH(res->header.cookies, de) {
            /* build cookie value str */
            stat = build_cookie(de->val, &tmpstr);
            if (stat == COOKIE_ERROR) 
                return HTTP_ERROR;
            /* build cookie header line */
            tmpstrlen = xasprintf(&tmpstr, "Set-Cookie: %s\r\n", tmpstr);
            /* send cookie header line */
            stat = bksmt_conn_msend(conn, tmpstr, tmpstrlen);
            free(tmpstr);
            if (stat == CONN_ERROR)
                return HTTP_ERROR;
        }
    }

    /* send header terminator */
    stat = bksmt_conn_msend(conn, dcrlf, 2);
    if (stat == CONN_ERROR) 
        return HTTP_ERROR;
 
    /* send body */
    if (res->body) {
        stat = bksmt_conn_msend(conn, res->body, res->blen);
        if (stat == CONN_ERROR)
            return HTTP_ERROR;
    }

    return HTTP_OK;
}

static void
bksmt_http_res_clear(struct bksmt_http_res *res)
{
    assert(res != NULL);

    if (res->header.mfields) 
        bksmt_dict_free(res->header.mfields);

    if (res->header.cookies) 
        bksmt_dictcase_free(res->header.cookies);

    if (res->body)
        free(res->body);
}

void
bksmt_http_res_free(struct bksmt_http_res *res)
{
    assert(res != NULL);

    bksmt_http_res_clear(res);
    free(res);
}
