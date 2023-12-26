#include "client.h"

#include "../net/conn.h"
#include "../lib/buf.h"
#include "../lib/dict.h"
#include "../lib/dictcase.h"
#include "../lib/llkv.h"
#include "http.h"
#include "uri.h"
#include "request.h"
#include "response.h"
#include "../lib/xmalloc.h"
#include "../lib/xstring.h"
#include "../lib/strconv.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

struct bksmt_http_client *
bksmt_http_client_init() 
{
    struct bksmt_http_client *client;

    client = xzalloc(sizeof *client);
    client->conn = NULL;
    client->cookiejar = bksmt_llkv_init(); 
    return client;
}

int 
bksmt_http_client_do(struct bksmt_http_client *client, 
        struct bksmt_http_req *req, int flags, struct bksmt_http_res **res)
{

    char *domain, *scheme, *port, *cfield, *auth, *cookief, *val;
    struct bksmt_uri *uri;
    struct bksmt_http_prot_lut_entry he;
    struct bksmt_dictcase *cookies;
    struct bksmt_dictcase_elem *dce;
    int stat, conntype, connflags;

    assert(client != NULL);
    assert(req != NULL && req->header.fpath != NULL);
    assert(res != NULL);


    /* init vars */
    cookief = NULL;
    uri = NULL;
    port = NULL;
    connflags = 0;

    /* init mfields for cookie */
    if (!req->header.mfields)
        req->header.mfields = bksmt_dict_init();
 
    /* 
     * we first have to figure out address of receiver. 
     * in order to do this, we need to first analyze 
     * the form of uri.
     */
    
    /* 
     * if fpath is '*' this is OPTIONS and we have 
     * to look in mime fields for host + scheme info,
     * if fpath begins with '/' then it is in relative
     * form and we have to look in mime fields also 
     */
    if (req->header.fpath[0] == '/' || req->header.fpath[0] == '*') {

        /* look for host */
        domain = bksmt_dict_get(req->header.mfields, "Host");

        /* if no host field, we have no way of discerning receiver */
        if (domain == NULL)
            return HTTP_ERROR;

        /* look for scheme */
        scheme = bksmt_dict_get(req->header.mfields, "Scheme");

        /* default to http */
        if (scheme == NULL)
            scheme = "http";

        /* determine port */
        if (strcasecmp(scheme, "https"))
            port = "443";
        else 
            port = "80";

    } else {
        /* 
         * reach here if fpath is in absolute form, we can
         * perform uri analysis using uri lib
         */
        /* init uri struct and parse fpath */
        stat = bksmt_uri_parse(req->header.fpath, &uri);

        /* if parsing failed, fpath is not valid uri, fail */
        if (stat != HTTP_OK) {
            bksmt_uri_free(uri);
            return HTTP_ERROR;
        }

        /* extract domain and scheme, we free uri later */
        domain = uri->dn;
        scheme = bksmt_http_prot_lut[uri->protocolk].prot; 
        if (uri->port != 0) 
            port = inttocstr(uri->port); 
        else if (uri->protocolk == HTTP_HTTP) 
            port = "80";
        else 
            port = "443";
    }

    if ('0' > domain[0] || domain[0] > '0' + 9)
        connflags |= CONN_DNS;

    /* determine conn type */
    if (port != NULL && strcmp(port, "80") == 0) 
        conntype = CONN_TCP;
    else 
        conntype = CONN_TLS;


    /* determine if we need to manage conn */
    if (client->cdomain == NULL || 
            strcmp(client->cdomain, domain) || 
            client->cscheme == NULL ||
            strcasecmp(client->cscheme, scheme) ||
            client->cport == NULL ||
            strcmp(client->cport, port)) {
        if (client->conn)
            bksmt_conn_free(client->conn);
        stat = bksmt_conn_init(domain, port, conntype, connflags, &(client->conn));
        if (stat != CONN_OK) {
            stat = HTTP_ERROR;
            goto abort;
        }

        if (client->cdomain) 
            free(client->cdomain);
        if (client->cport) 
            free(client->cport);
        if (client->cscheme) 
            free(client->cscheme);
        client->cdomain = xstrdup(domain);
        client->cscheme = xstrdup(scheme);
        client->cport = xstrdup(port);
    }

    /* make authority, we use this as key in llkv */
    xasprintf(&auth, "%s://%s:%s", client->cscheme, client->cdomain, client->cport);

    /* make cookie header */
    cookies = bksmt_llkv_get(client->cookiejar, auth, 0);
    BKSMT_DICTCASE_FOREACH(cookies, dce) {
        val = bksmt_dict_get(dce->val, "Value");
        if (cookief == NULL)
            xasprintf(&cookief, "%s=%s", dce->key, val); 
        else
            xasprintf(&cookief, "%s; %s=%s", dce->key, val); 
    }

    if (cookief != NULL)
        bksmt_dict_set(req->header.mfields, "Cookie", cookief);
 
    /* open conn if closed */
    if (!bksmt_conn_is_open(client->conn)) {
        stat = bksmt_conn_open(client->conn);
        if (stat != CONN_OK) {
            stat = HTTP_ERROR;
            goto abort1;
        }
    }

    /* send req */
    stat = bksmt_http_req_send(req, client->conn);
    if (stat != CONN_OK) {
        stat = HTTP_ERROR;
        goto abort1;
    }

    /* alloc res and recv  */
    *res = xmalloc(sizeof **res);
    stat = bksmt_http_res_recv(*res, client->conn);
    if (stat != CONN_OK) {
        stat = HTTP_ERROR;
        goto abort2;
    }

    cfield = bksmt_dict_get((*res)->header.mfields, "Connection");
    /* if connection field says close, then close */
    if (cfield == NULL || strcasecmp(cfield, "close") == 0) 
        bksmt_conn_close(client->conn);

    if ((*res)->header.cookies != NULL && !(flags & HTTP_CLIENT_DO_NOCOOK)) {
        bksmt_dictcase_apply(cookies, (*res)->header.cookies);
        /* no longer need this */
    }

    free(auth);
    stat = HTTP_OK;
    goto done;
 
abort2:
    bksmt_http_res_free(*res);
abort1:
    free(auth);
abort:
    if (client->conn) 
        bksmt_conn_free(client->conn);
done:
    if(uri && uri->port > 0)
        free(port);
    if(uri)
        bksmt_uri_free(uri);
    return stat;
}

struct bksmt_dictcase *
bksmt_http_client_get_cookies(struct bksmt_http_client *client, char *authority)
{
    return bksmt_llkv_get(client->cookiejar, authority, 0);
}

void bksmt_http_client_free(struct bksmt_http_client *client) 
{
    assert(client != NULL);

    /* if we have a conn, close if necessary and free */
    if (client->conn != NULL) 
        bksmt_conn_free(client->conn);

    /* if we have a cookiejar, free */
    if (client->cookiejar != NULL)
        bksmt_llkv_free(client->cookiejar);

    /* free all conn tracking state */
    if (client->cdomain)
        free(client->cdomain);
    if (client->cport)
        free(client->cport);
    if (client->cscheme)
        free(client->cscheme);

    /* free holding client */
    free(client);
}
