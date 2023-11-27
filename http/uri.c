#include "uri.h"

#include "http.h"
#include "../lib/xstring.h"
#include "../lib/strconv.h"
#include "../lib/xmalloc.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * locals for parsing
 */

/* parse status */
#define HTTP_URI_PARSE_NOPROT   1
#define HTTP_URI_PARSE_END      2
#define HTTP_URI_PARSE_PORT     3
#define HTTP_URI_PARSE_PATH     4
#define HTTP_URI_PARSE_PARAM    5
#define HTTP_URI_PARSE_ANCHOR   6
#define HTTP_URI_PARSE_ERROR    7
#define HTTP_URI_PARSE_OK       8

static int   parse_prot(char **, int *); 
static int   parse_dn(char **, char **); 
static int   parse_fpath(char **, char **); 
static int   parse_port(char **, int *); 
static int   parse_parameters(char **, struct bksmt_dict **); 

/* build locals */

#define BKSMT_ISPCTENC(flags) flags & BKSMT_URI_PARSE_PCTENC

/*
 * should optimize
 */
struct bksmt_pctenc_lut_entry {
    char orig;
    char enc[4]; 
} bksmt_pctenc_lut[21] = {
    { ' ',  "%20" },
    { '!',  "%21" },
    { '\"', "%22" },
    { '#',  "%23" },
    { '$',  "%24" },
    { '%',  "%25" },
    { '&',  "%26" },
    { '\'', "%27" },
    { '(',  "%28" },
    { ')',  "%29" },
    { '*',  "%2A" },
    { '+',  "%2B" },
    { ',',  "%2C" },
    { '/',  "%2F" },
    { ':',  "%3A" },
    { ';',  "%3B" },
    { '=',  "%3D" },
    { '\?', "%3F" },
    { '@',  "%40" },
    { '[',  "%5B" },
    { ']',  "%5D" },
};

/*
 * bksmt_cstrpctenc percent encodes
 * the string "raw" on
 * a heap allocated returned
 * string
 */
char *
bksmt_cstrpctenc(char *raw)
{
    int i, j;
    char *ret = strdup(raw);

    i = 0;
    while(ret[i] != 0) {
        /*
         * loop through all
         * special chars to see
         * if we match
         */
        for (j = 0; j < 21; j++) {
            if (bksmt_pctenc_lut[j].orig == ret[i]) {
                xasprintf(&ret, "%.*s%.*s%s", 
                        i, ret, 3, bksmt_pctenc_lut[j].enc, ret + i + 1);
                i += 2;
                continue;
            }
        }
        i++;
    }
    return ret;
}

static int 
parse_prot(char **pstr, int *protk)
{
    struct bksmt_http_prot_lut_entry h, hs;

    /* get str info on protocols */
    h = bksmt_http_prot_lut[HTTP_HTTP];
    hs = bksmt_http_prot_lut[HTTP_HTTPS];

    /* check which prot we have */
    if (strncasecmp(*pstr, hs.prot, hs.len) == 0) {
        *protk = HTTP_HTTPS;
        *pstr += hs.len;
        return HTTP_URI_PARSE_OK;
    } else if (strncasecmp(*pstr, h.prot, h.len) == 0) {
        *protk = HTTP_HTTP;
        *pstr += h.len;
        return HTTP_URI_PARSE_OK;
    }

    /* if none assume this is no prot */
    return HTTP_URI_PARSE_NOPROT;
}

static int
parse_dn(char **pstr, char **dn)
{
    char *fslash, *fcol, *finq, *fhash;
    size_t cplen;

    /* find first colon */
    fcol = strchr(*pstr, ':'); 

    /* if nothing in between protocol and colon, err */
    if (fcol == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;

    /* if we have a col, we have a port. scan to the col */
    if (fcol != NULL) {
        cplen = fcol - *pstr;
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PORT;
    }

    /* if no col we may have a path */
    fslash = strchr(*pstr, '/'); 

    /* if the / and curr ptr have nothing in between, err */
    if (fslash == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;
 
    /* if we have a slash, we have a path. scan to the slash */
    if (fslash != NULL) {
        cplen = fslash - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PATH;
    }

    /* if no / we may have queries */
    finq = strchr(*pstr, '?'); 

    /* checking no dn again */
    if (finq == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;

    /* scan till next token */
    if (finq != NULL) {
        cplen = finq - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PARAM;
    }

    /* if not ? we may have an anchor */
    fhash = strchr(*pstr, '#'); 

    /* checking no dn again */
    if (fhash == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;

    /* scan till next token */
    if (fhash != NULL) {
        cplen = fhash - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_ANCHOR;
    }

    /* checking no dn again */
    if ((*pstr)[0] == 0)
        return HTTP_URI_PARSE_ERROR;
 
    /* scan till next end */
    *dn = strdup(*pstr);
    *pstr += strlen(*pstr);
    return HTTP_URI_PARSE_END;
}

static int
parse_port(char **pstr, int *port) 
{
    char *fslash, *finq, *fhash;

    /* check next token */
    fslash = strchr(*pstr, '/');

    /* check for if there is any port */
    if (fslash == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;
 
    /* if we have port scan it */
    if (fslash != NULL) {
       *port = csubstrtoint(*pstr, fslash);
        *pstr += fslash - *pstr;
        return HTTP_URI_PARSE_PATH;
    }

    /* check next token */
    finq = strchr(*pstr, '?'); 
    
    /* check for if there is any port */
    if (finq == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;
 
    /* if we have port scan it */
    if (finq != NULL) {
        *port = csubstrtoint(*pstr, finq);
        *pstr += finq - *pstr;
        return HTTP_URI_PARSE_PARAM;
    }

    /* check next token */
    fhash = strchr(*pstr, '#'); 

    /* check for if there is any port */
    if (fhash == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;

    /* if we have port scan it */
    if (fhash != NULL) {
        *port = csubstrtoint(*pstr, fhash);
        *pstr += fhash - *pstr;
        return HTTP_URI_PARSE_ANCHOR;
    }

    /* check for if there is any port */
    if ((*pstr)[0] == 0)
        return HTTP_URI_PARSE_ERROR;

    /* if we have port scan it */
    *port = cstrtoint(*pstr);
     return HTTP_URI_PARSE_END;
}

static int   
parse_fpath(char **pstr, char **fpath) 
{
    char *fhash, *finq;
    size_t cplen;

    /* check next token */
    finq = strchr(*pstr, '?');

    /* check for if there is any path */
    if (finq == *pstr + 1)
       return HTTP_URI_PARSE_ERROR;

    /* if we have any path scan it */
    if (finq != NULL) {
        cplen = finq - *pstr;
        xasprintf(fpath, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PARAM;
    }

    /* check next token */
    fhash = strchr(*pstr, '#');

    /* check for if there is any path */
    if (fhash == *pstr + 1)
        return HTTP_URI_PARSE_ERROR;

    /* if we have any path scan it */
    if (fhash != NULL) {
        cplen = fhash - *pstr;
        xasprintf(fpath, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_ANCHOR;
    }

    /* check for if there is any path */
    if ((*pstr)[0] == 0)
        return HTTP_URI_PARSE_ERROR;

    /* if we have any path scan it */
    *fpath = strdup(*pstr);
    *pstr += strlen(*pstr);
    return HTTP_URI_PARSE_END;
}

static int
parse_parameters(char **pstr, struct bksmt_dict **params) 
{
    char *nxteq, *nxtamp, *fhash;
    char *tmpk, *tmpv;
    int stat;
    
    *params = bksmt_dict_init();

    /* find position of next equals */
    while((nxteq = strchr(*pstr, '='))) {
        /* if nothing is in the key spot, err*/
        if (nxteq == *pstr) {
            stat = HTTP_URI_PARSE_ERROR;
            goto pquit;
        }
        xasprintf(&tmpk, "%.*s", nxteq - *pstr, *pstr);
        *pstr = nxteq + 1; 
        nxtamp = strchr(*pstr, '&');
        /* if there is no amp, we have no more query pairs */
        if (nxtamp == NULL) {
            /* check if there is a hash, and scan up to it */
            fhash = strchr(*pstr, '#');
            if (fhash == NULL) {
                /* if there isn't a hash and we're at end, err */
                if ((*pstr)[0] == 0) {
                    stat = HTTP_URI_PARSE_ERROR;
                    goto perror;
                }
                /* scan till end of str */
                tmpv = strdup(*pstr);
                bksmt_dict_set(*params, tmpk, tmpv);
                stat = HTTP_URI_PARSE_END;
                goto finish;
            }
            /* if we have a hash nothing in between, err*/
            if (fhash == *pstr + 1) {
                stat = HTTP_URI_PARSE_ERROR;
                goto perror;
            }
            xasprintf(&tmpv, "%.*s", fhash - *pstr, *pstr);
            *pstr = fhash;
            bksmt_dict_set(*params, tmpk, tmpv);
            stat = HTTP_URI_PARSE_ANCHOR;
            goto finish;
        }

        /* if we have an amp and nothing in between, err*/
        if (nxtamp == nxteq + 1) {
            stat = HTTP_URI_PARSE_ERROR;
            goto perror;
        }
        xasprintf(&tmpv, "%.*s", nxtamp - *pstr, *pstr);
        *pstr = nxtamp + 1;
        bksmt_dict_set(*params, tmpk, tmpv);
        free(tmpk);
        free(tmpv);
    }

finish:
    free(tmpv);
perror:
    free(tmpk);
pquit:
    return stat;
}

int 
bksmt_uri_parse(struct bksmt_uri *dst, char *src)
{
    char *end;
    int stat, protk, port;
    struct bksmt_dict *params;
    struct bksmt_dict_elem *e;

    assert(dst != NULL);

    /* clear all fields of the dst uri struct */
    dst->dn = dst->fpath = dst->anchor = NULL;
    dst->parameters = NULL;
    dst->port = 0;

    end = strlen(src) + src;

    /* start by parsing protection */
    stat = parse_prot(&src, &(dst->protocolk));
    switch(stat){
    case HTTP_URI_PARSE_NOPROT:
        break;
    case HTTP_URI_PARSE_OK:
        if (src + 3 >= end)
            goto error;
        src += 3;
        break;
    case HTTP_URI_PARSE_ERROR:
        goto error;
    }    

    /* only choice if no err is to parse dn */
    stat = parse_dn(&src, &(dst->dn));
    switch(stat){
    case HTTP_URI_PARSE_ERROR:
        goto error;
    case HTTP_URI_PARSE_PORT:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto port;
    case HTTP_URI_PARSE_PATH:
        goto path;
    case HTTP_URI_PARSE_END:
        goto end;
    case HTTP_URI_PARSE_PARAM:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto param;
    case HTTP_URI_PARSE_ANCHOR:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto anchor;
    }

port:    
    stat = parse_port(&src, &(dst->port));
    switch(stat) {
    case HTTP_URI_PARSE_PARAM:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto param;
    case HTTP_URI_PARSE_ANCHOR:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto anchor;
    case HTTP_URI_PARSE_PATH:
        goto path;
    case HTTP_URI_PARSE_END:
        goto end;
    case HTTP_URI_PARSE_ERROR:
        goto error;
    }
 
path:
    stat = parse_fpath(&src, &(dst->fpath));
    switch(stat) {
    case HTTP_URI_PARSE_ERROR:
        goto error;
    case HTTP_URI_PARSE_PARAM:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto param;
    case HTTP_URI_PARSE_ANCHOR:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto anchor;
    case HTTP_URI_PARSE_END:
        goto end;
    } 

param:
    stat = parse_parameters(&src, &(dst->parameters));
    switch(stat) {
    case HTTP_URI_PARSE_ANCHOR:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto anchor;
    case HTTP_URI_PARSE_END:
        goto end;
    case HTTP_URI_PARSE_ERROR:
        goto error;
    } 
anchor:
     dst->anchor = strdup(src);
     goto end;

error:
     bksmt_uri_clear(dst);
     return HTTP_ERROR;

end:
     return HTTP_OK;
}

int
bksmt_uri_build(struct bksmt_uri *uri, char **ret, int flags)
{
    struct bksmt_http_prot_lut_entry p;
    char *portstr;
    struct bksmt_dict_elem *e;
    size_t len, i;
    char *tmpk, *tmpv;

    assert(uri != NULL);
    assert(uri->dn != NULL);
    assert(uri->protocolk < 2);

    /* fill prootcol */
    p = bksmt_http_prot_lut[uri->protocolk];
    xasprintf(ret, "%.*s://", p.len, p.prot);

    /* fill domain name */
    xasprintf(ret, "%s%s", *ret, uri->dn);

    /* fill port */
    if (uri->port) {
        portstr = inttocstr(uri->port);
        xasprintf(ret, "%s:%s", *ret, portstr);
        free(portstr);
    }

    /* fill path */
    xasprintf(ret, "%s%s", *ret, uri->fpath ? uri->fpath : "/");

    /* fill parameters */
    if (uri->parameters) {
        xasprintf(ret, "%s?", *ret);
        i = 0;
        len = uri->parameters->nelem;
        BKSMT_DICT_FOREACH(uri->parameters, e) {
            if (BKSMT_ISPCTENC(flags)) {
                tmpk = bksmt_cstrpctenc(e->key);
                tmpv = bksmt_cstrpctenc(e->val);
            } else {
                tmpk = e->key;
                tmpv = e->val;
            }
            if (i == len - 1)
                xasprintf(ret, "%s%s=%s", *ret, tmpk, tmpv);
            else
                xasprintf(ret, "%s%s=%s&", *ret, tmpk, tmpv);

            if (BKSMT_ISPCTENC(flags)) {
                free(tmpk);
                free(tmpv);
            }

            i++;
        }
    }

    /* fill anchor */
    if (uri->anchor) {
        xasprintf(ret, "%s#%s", *ret, uri->anchor);
    }
    return HTTP_OK;
}

void
bksmt_uri_clear(struct bksmt_uri *uri)
{
    assert(uri != NULL);
    if (uri->dn) free(uri->dn);
    if (uri->fpath) free(uri->fpath);
    if (uri->parameters) bksmt_dict_free(uri->parameters);
    if (uri->anchor) free(uri->anchor);
}

void
bksmt_uri_free(struct bksmt_uri *uri)
{
    assert(uri != NULL);
    bksmt_uri_clear(uri);
    free(uri);
}
