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

char *
bksmt_cstrpctenc(char *raw)
{
    int i, j;
    char *ret = strdup(raw);

    i = 0;
    while(ret[i] != 0) {
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

struct bksmt_uri *
bksmt_uri_init(void)
{
    return xmalloc(sizeof (struct bksmt_uri));
}


/* static int   parse_method(char **, int *); 
 * static int   parse_dn(char **, char **); 
 * static int   parse_fpath(char **, char **); 
 * static int   parse_port(char **, int *); 
 * static int   parse_parameters(char **, int, struct bksmt_dict **); 
 * static int   parse_anchor(char **, struct bksmt_dict *); 
 */

static int 
parse_prot(char **pstr, int *protk)
{
    struct bksmt_http_prot_lut_entry h, hs;

    h = bksmt_http_prot_lut[HTTP_HTTP];
    hs = bksmt_http_prot_lut[HTTP_HTTPS];

    if (strncasecmp(*pstr, hs.prot, hs.len) == 0) {
        *protk = HTTP_HTTPS;
        *pstr += hs.len;
        return HTTP_URI_PARSE_OK;
    } else if (strncasecmp(*pstr, h.prot, h.len) == 0) {
        *protk = HTTP_HTTP;
        *pstr += h.len;
        return HTTP_URI_PARSE_OK;
    }
    return HTTP_URI_PARSE_NOPROT;
}

static int
parse_dn(char **pstr, char **dn)
{
    char *fslash, *fcol, *finq, *fhash;
    size_t cplen;

    fcol = strchr(*pstr, ':'); 

    /*
     * if there is a :, we have
     * a uri of the form
     * www.google.com:80 ..., so copy
     * up to :
     */
    if (fcol != NULL) {
        cplen = fcol - *pstr;
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PORT;
    }

    fslash = strchr(*pstr, '/'); 

    /*
     * if there is a / and no :, we have
     * a uri of the form
     * www.google.com/path, so copy
     * up to /
     */
    if (fslash != NULL) {
        cplen = fslash - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PATH;
    }

    finq = strchr(*pstr, '?'); 

    /*
     * if there is a ? and no : /, we have
     * a uri of the form
     * www.google.com?params, so copy
     * up to ?
     */
    if (finq != NULL) {
        cplen = finq - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PARAM;
    }

    fhash = strchr(*pstr, '#'); 

    /*
     * if there is a # and no : / ?, we have
     * a uri of the form
     * www.google.com#anchor, so copy
     * up to #
     */
    if (fhash != NULL) {
        cplen = fhash - *pstr; 
        xasprintf(dn, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_ANCHOR;
    }

    *dn = strdup(*pstr);
    *pstr += strlen(*pstr);
    return HTTP_URI_PARSE_END;
}

static int
parse_port(char **pstr, int *port) 
{
    char *fslash, *finq, *fhash;

    fslash = strchr(*pstr, '/');

    if (fslash != NULL) {
        *port = csubstrtoint(*pstr, fslash);
        *pstr += fslash - *pstr;
        return HTTP_URI_PARSE_PATH;
    }

    finq = strchr(*pstr, '?'); 

    if (finq != NULL) {
        *port = csubstrtoint(*pstr, finq);
        *pstr += finq - *pstr;
        return HTTP_URI_PARSE_PARAM;
    }

    fhash = strchr(*pstr, '#'); 

    if (finq != NULL) {
        *port = csubstrtoint(*pstr, fhash);
        *pstr += fhash - *pstr;
        return HTTP_URI_PARSE_ANCHOR;
    }

    *port = cstrtoint(*pstr);
     return HTTP_URI_PARSE_END;
 
}

static int   
parse_fpath(char **pstr, char **fpath) 
{
    char *fhash, *finq;
    size_t cplen;

    fhash = strchr(*pstr, '#');
    finq = strchr(*pstr, '?');
    
    /*
     * if there's no # or ?, then
     * path extends to the end of
     * the uri
     */
    if (fhash == NULL && finq == NULL) {
        *fpath = strdup(*pstr);
        *pstr += strlen(*pstr);
        return HTTP_URI_PARSE_END;
    }

    /*
     * if there is a ?,
     * we have params, so 
     * parse till the params
     */
    if (finq != NULL) {
        cplen = finq - *pstr;
        xasprintf(fpath, "%.*s", cplen, *pstr);
        *pstr += cplen;
        return HTTP_URI_PARSE_PARAM;
    }

    cplen = fhash - *pstr;
    xasprintf(fpath, "%.*s", cplen, *pstr);
    *pstr += cplen;
    return HTTP_URI_PARSE_ANCHOR;
}

static int
parse_parameters(char **pstr, struct bksmt_dict **params) 
{
    char *nxteq, *nxtamp, *fhash;
    char *tmpk, *tmpv;
    int stat;
    
    *params = bksmt_dict_init();

    while((nxteq = strchr(*pstr, '='))) {
        xasprintf(&tmpk, "%.*s", nxteq - *pstr, *pstr);
        *pstr = nxteq + 1; 
        nxtamp = strchr(*pstr, '&');
        if (nxtamp == NULL) {
            fhash = strchr(*pstr, '#');
            if (fhash == NULL) {
                tmpv = strdup(*pstr);
                bksmt_dict_set(*params, tmpk, tmpv);
                stat = HTTP_URI_PARSE_END;
                goto finish;
            }
            xasprintf(&tmpv, "%.*s", fhash - *pstr, *pstr);
            *pstr = fhash;
            bksmt_dict_set(*params, tmpk, tmpv);
            stat = HTTP_URI_PARSE_ANCHOR;
            goto finish;
        }
        xasprintf(&tmpv, "%.*s", nxtamp - *pstr, *pstr);
        *pstr = nxtamp + 1;
        bksmt_dict_set(*params, tmpk, tmpv);
        free(tmpk);
        free(tmpv);
    }

finish:
    free(tmpv);
    free(tmpk);
    return stat;
}

int 
bksmt_uri_parse(struct bksmt_uri **dst, char *src, int flags)
{
    char *end;
    int stat, protk, port;
    struct bksmt_dict *params;
    struct bksmt_dict_elem *e;

    end = strlen(src) + src;
    *dst = xzalloc(sizeof **dst);

    stat = parse_prot(&src, &((*dst)->protocolk));
    switch(stat){
    case HTTP_URI_PARSE_NOPROT:
        break;
    case HTTP_URI_PARSE_OK:
        if (src + 3 >= end)
            goto error;
        src += 3;
        break;
    case HTTP_URI_PARSE_ERROR:
        return HTTP_ERROR;
    }    
    stat = parse_dn(&src, &((*dst)->dn));
    switch(stat){
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
    stat = parse_port(&src, &((*dst)->port));
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
    }
 
path:
    stat = parse_fpath(&src, &((*dst)->fpath));
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
    case HTTP_URI_PARSE_END:
        goto end;
    } 

param:
    stat = parse_parameters(&src, &((*dst)->parameters));
    switch(stat) {
    case HTTP_URI_PARSE_ANCHOR:
        if (src + 1 >= end)
            goto error;
        src += 1;
        goto anchor;
    case HTTP_URI_PARSE_END:
        goto end;
    } 

anchor:
     (*dst)->anchor = strdup(src);
     goto end;

error:
     bksmt_uri_free(*dst);
     return HTTP_ERROR;

end:
     return HTTP_OK;
}

int
bksmt_uri_build(struct bksmt_uri *uri, char **ret)
{
    struct bksmt_http_prot_lut_entry p;
    char *portstr;
    struct bksmt_dict_elem *e;
    size_t len;

    assert(uri != NULL);
    assert(uri->dn != NULL);
    assert(uri->protocolk < 2);

    /*
     * fill in protocol
     */
    p = bksmt_http_prot_lut[uri->protocolk];
    xasprintf(ret, "%.*s://", p.len, p.prot);

    /*
     * fill in domain name 
     */
    xasprintf(ret, "%s%s", *ret, uri->dn);

    /*
     * fill in port 
     */
    if (uri->port) {
        portstr = inttocstr(uri->port);
        xasprintf(ret, "%s:%s", *ret, portstr);
        free(portstr);
    }

    /*
     * fill in path 
     */
    xasprintf(ret, "%s%s", *ret, uri->fpath ? uri->fpath : "/");

    /*
     * fill parameters
     */
    if (uri->parameters) {
        xasprintf(ret, "%s?", *ret);
        BKSMT_DICT_FOREACH(uri->parameters, e) {
            xasprintf(ret, "%s%s=%s&", *ret, e->key, e->val);
        }
        len = strlen(*ret);
        /*
         * delete last ampersand
         */
        (*ret)[len-1] = 0;
    }

    if (uri->anchor) {
        xasprintf(ret, "%s#%s", *ret, uri->anchor);
    }
    return HTTP_OK;
}

void
bksmt_uri_free(struct bksmt_uri *uri)
{
    assert(uri != NULL);
    if (uri->dn) free(uri->dn);
    if (uri->fpath) free(uri->fpath);
    if (uri->parameters) bksmt_dict_free(uri->parameters);
    if (uri->anchor) free(uri->anchor);
    free(uri);
}
