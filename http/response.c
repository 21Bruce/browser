#include "response.h"

#include "../net/conn.h"
#include "../lib/xstring.h"
#include "../lib/bufread.h"
#include "../lib/strconv.h"
#include "../lib/xmalloc.h"
#include "mime.h"
#include "http.h"
#include "cookie.h"

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define RBUFLEN 8000

/* body buffered reader inf */

struct bksmt_http_chunk_tap {
    /* aic -- amount in chunk, 0 if we need to read in a new chunk, and
     * positive definite representing how much left in a chunk
     * if we are in the middle of reading a chunk  */
    uint64_t               aic;
    /* eof flag to prevent undefined reads */
    int                    eof;
    /* connection to source from */
    struct bksmt_conn      *conn;
};
static int bksmt_http_chunk_read(void *, unsigned char *, int *);
static void bksmt_http_chunk_free(void *);

struct bksmt_http_content_length_tap {
    /* cll -- content left length, positive definite 
     * representing how much left to read from content */
    uint64_t               cll;
    /* connection to source from */
    struct bksmt_conn      *conn;
};
static int bksmt_http_content_length_read(void *, unsigned char *, int *);
static void bksmt_http_content_length_free(void *);


/* locals for parsing */
static int parse_header(struct bksmt_http_res *, unsigned char *, size_t); 
static int inttostatk(int);


static char *adv_ws(char *, char *);

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
    struct bksmt_dict *cookie;
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
    char rbuf[RBUFLEN];
    int pos, stat, r1f, n1f, r2f, n2f, endf;
    size_t hlen, chlen, blen;
    char *lenstr, *cfield; 
    struct bksmt_http_chunk_tap *chtap;
    struct bksmt_http_content_length_tap *cltap;

    assert(res != NULL && conn != NULL);

    /* init res body and bufbck */
    res->body = NULL;

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

    /* need this to determine cleanup func for body buffered reader */
    cfield = bksmt_dict_get(res->header.mfields, "Connection");

    /* if there is a content length, establish content-length buffered reader */
    if((lenstr = bksmt_dict_get(res->header.mfields, "Content-Length"))) {
        cltap = xzalloc(sizeof *cltap);
        cltap->cll = cstrtoint(lenstr);
        cltap->conn = conn;
        /* if connection field says close, then close */
        if (cfield == NULL || strcasecmp(cfield, "close") == 0) 
            res->body = bksmt_bufread_init(cltap, bksmt_http_content_length_read, bksmt_http_content_length_free); 
        else
            res->body = bksmt_bufread_init(cltap, bksmt_http_content_length_read, free); 
        return HTTP_OK;
    }

    /* if there is a chunked transfer encoding, parse it and read it in */
    if (strcasecmp(bksmt_dict_get(res->header.mfields, "Transfer-Encoding"), "chunked") == 0) {
        chtap = xzalloc(sizeof *chtap);
        chtap->aic = 0;
        chtap->eof = 0;
        chtap->conn = conn;
        if (cfield == NULL || strcasecmp(cfield, "close") == 0) 
            res->body = bksmt_bufread_init(chtap, bksmt_http_chunk_read, bksmt_http_chunk_free); 
        else
            res->body = bksmt_bufread_init(chtap, bksmt_http_chunk_read, free); 
        return HTTP_OK;
    }

    return HTTP_ERROR;
}


int
bksmt_http_res_send(struct bksmt_http_res *res, struct bksmt_conn *conn)
{
    char sp, sl, *dcrlf, *tmpstr;
    size_t tmpstrlen;
    struct bksmt_http_status_lut_entry sk;
    struct bksmt_dict_elem *e;
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
 
    /* COME BACK */
    /* send body */
//    if (res->body) {
//        stat = bksmt_conn_msend(conn, res->body, res->blen);
//        if (stat == CONN_ERROR)
//            return HTTP_ERROR;
//    }

    return HTTP_OK;
}

static int 
bksmt_http_content_length_read(void *tap, unsigned char *out, int *size)
{
    /*
     * This function must output *size bytes of the http
     * body at out buffer, or as many bytes till an EOF if there are less bytes
     * than *size remaining. We use this function if the body is in the content
     * length format. In this case, we always know the max size of the message,
     * stored in variable tap->cll.
     *
     */

    /* value cast of tap */
    struct bksmt_http_content_length_tap *vtap;

    /* storage of func statuses */
    int stat;

    vtap = (struct bksmt_http_content_length_tap *)tap;

    /* if we do not have any content, return EOF */
    if (vtap->cll == 0) {
        *size = 0; 
        return BKSMT_BUFREAD_EOF;
    }

    /* case, we have enough content to source request */
    if (*size < vtap->cll) {
        stat = bksmt_conn_mrecv(vtap->conn, out, *size);
        if (stat == CONN_ERROR)
            goto error;
        vtap->cll -= *size;
        return BKSMT_BUFREAD_OK;
    }

    /* case, we do not have enough content to source request */
    stat = bksmt_conn_mrecv(vtap->conn, out, vtap->cll);
    if (stat == CONN_ERROR)
        goto error;
    *size = vtap->cll;
    vtap->cll = 0;
    return BKSMT_BUFREAD_EOF;

error:
    *size = 0;
    return BKSMT_BUFREAD_ERR;
}

static void 
bksmt_http_content_length_free(void *tap)
{
    struct bksmt_http_content_length_tap *vtap;

    vtap = (struct bksmt_http_content_length_tap *)tap; 
    bksmt_conn_close(vtap->conn);
    free(vtap);
}

static int 
bksmt_http_chunk_read(void *tap, unsigned char *out, int *size)
{

    /*
     * This function must output *size bytes of the http
     * body at out buffer, or as many bytes till an EOF if there are less bytes
     * than *size remaining. We use this function if the body is in the chunked
     * transfer encoding format.
     *
     * The chunked transfer encoding is composed of multiple chunks of the form 
     *  ... \r\n CHUNKSIZE \r\n data[CHUNKSIZE] ... 
     * Where the end of the message has format
     *  ... \r\n 0 \r\n \r\n ... 
     *
     * The tap variable contains the connection and a 64-bit uint "aic" representing
     * the amount left in the current chunk. If it is zero, then we need to process 
     * in the "\r\n CHUNKSIZE \r\n" part of the sequence. We always read in the 
     * first "\r\n" part of the sequence after processing a chunk for convenience,
     * so we have to read "CHUNKSIZE \r\n". We read as many chunks as necessary
     * to source the req. If we end in the middle of a chunk, we set aic to the # of
     * remaining bytes in the chunk.
     */


    /* value cast of tap */
    struct bksmt_http_chunk_tap *vtap;

    /* rsize -- size we have read in so far */
    uint64_t rsize;

    /* stat -- storage of func statuses */
    int stat, i;

    /* chlenbuf -- we use this to read in the length of the chunk before converting */
    /* garbage -- we use this to read in crlf sequences that we don't need */
    unsigned char chlenbuf[sizeof(uint64_t)], garbage[2];

    /* init vals */
    vtap = (struct bksmt_http_chunk_tap *)tap;
    rsize = 0;

    /* if we do not have any content, return EOF */
    if (vtap->eof) {
        *size = 0; 
        return BKSMT_BUFREAD_EOF;
    }

    /* keep reading while we haven't read in proper size. handle EOF separately */
    while (rsize != *size) {
        /* in this case, we need to start processing a new chunk */
        if (vtap->aic == 0) {

            /* read length of chunk string into chlenbuf */
            stat = bksmt_conn_mrecv(vtap->conn, chlenbuf, 1);
            for(i = 0; chlenbuf[i] != '\r'; i++) {
                if (stat != CONN_OK || i + 1 >= sizeof(uint64_t)) 
                    goto error;
                stat = bksmt_conn_mrecv(vtap->conn, chlenbuf + i + 1, 1);
            }

            /* in the last for loop, we read in the CR of the CRLF seq
             * so we need to read in the LF */
            stat = bksmt_conn_mrecv(vtap->conn, garbage, 1);
            if (stat == CONN_ERROR) 
                goto error;
            /* convert chlenbuf to an actual chunk length number */
            vtap->aic = hexcsubstrtoint(chlenbuf, chlenbuf + i);
            /* if vtap->aic == 0, this is the "end of transmission" chunk */
            if (vtap->aic == 0) {
                /* read in ending CRLF to garbage */
                stat = bksmt_conn_mrecv(vtap->conn, garbage, 2);
                if (stat == CONN_ERROR) 
                    goto error;

                /* set eof flag */
                vtap->eof = 1;

                /* return size and eof */
                *size = rsize;
                return BKSMT_BUFREAD_EOF;
            }
        }
        
        /* case, we have to stop in middle of a chunk */
        if (rsize + vtap->aic > *size) {
            /* read in enough to source req */
            stat = bksmt_conn_mrecv(vtap->conn, out + rsize, *size - rsize);
            if (stat == CONN_ERROR) 
                goto error;
            /* update aic and return */
            vtap->aic -= *size - rsize;
            return BKSMT_BUFREAD_OK;
        }

        /* case, have to read whole chunk */
        stat = bksmt_conn_mrecv(vtap->conn, out + rsize, vtap->aic);
        if (stat == CONN_ERROR) 
            goto error;
        /* read in CRLF of next chunk */
        stat = bksmt_conn_mrecv(vtap->conn, garbage, 2);
        if (stat == CONN_ERROR) 
            goto error;

        /* add what we read in */
        rsize += vtap->aic;
        /* since we read in a whole chunk, this is 0 */
        vtap->aic = 0;
    }

error:
    *size = 0;
    return BKSMT_BUFREAD_ERR;
}

static void 
bksmt_http_chunk_free(void *tap)
{
    struct bksmt_http_chunk_tap *vtap;

    vtap = (struct bksmt_http_chunk_tap *)tap; 
    bksmt_conn_close(vtap->conn);
    free(vtap);
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
