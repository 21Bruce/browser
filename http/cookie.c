#include "cookie.h"

#include "../lib/dict.h"
#include "../lib/dictcase.h"
#include "../lib/xmalloc.h"
#include "../lib/xstring.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

/* parse locals */
static char *adv_ws(char *);

static char *
adv_ws(char *src)
{
    while (*src == ' ')
        src++;

    return src;
}

int
parse_cookie(char *src, struct bksmt_dict **cookie)
{
    char *nxteq, *nxtscol, *tmpk, *tmpv;

    assert(src != NULL);
    assert(cookie != NULL);

    *cookie = bksmt_dict_init();

    /* loop through cookie attrs, each attr has = but not necessarily ; */
    while((nxteq = strchr(src, '=')) != NULL) {

        /* init tmp buffers */
        tmpk = NULL;
        tmpv = NULL;
 
        /* adv cursor through ws */
        src = adv_ws(src);

        /* if cursor is at eq, then key = ws, so err */
        if (src == nxteq)
            goto kerror;

        /* cpy key into dynam buffer */
        xasprintf(&tmpk, "%.*s", nxteq - src, src);

        /* move cursor past equal, we don't need adv_ws since RFC says no ws here*/
        src = nxteq + 1;

        nxtscol = strchr(src, ';');

        /* if no next semicol, cpy to end. else, cpy to semicol */
        if (nxtscol == NULL) 
            xasprintf(&tmpv, "%s", src);
        else 
            xasprintf(&tmpv, "%.*s", nxtscol - src, src);

        /* if no 'Name' field, use this k-v */
        if (bksmt_dict_get(*cookie, "Name") == NULL) {
            bksmt_dict_set(*cookie, "Name", tmpk);
            bksmt_dict_set(*cookie, "Value", tmpv);
        } else {
        /* otherwise, this is a attr for this cookie */
            bksmt_dict_set(*cookie, tmpk, tmpv);
        }

        /* release dynam buffers */
        free(tmpk);
        free(tmpv);

        /* if nxtscol is NULL, terminate parsing */
        if (nxtscol == NULL)
            break;

        /* advance cursor past current k-v pair */
        src = nxtscol + 1;
    }

    if (bksmt_dict_get(*cookie, "Name") == NULL)
        goto kerror;

    return COOKIE_OK;

kerror:
    /* error getting key */
    bksmt_dict_free(*cookie);
    *cookie = NULL;
    return COOKIE_ERROR;
}

int
build_cookie(struct bksmt_dict *cookie, char **ret)
{
    struct bksmt_dict_elem *e;
    char *name, *val;

    assert(cookie != NULL);

    /* if Name or Value is missing this is not a valid cookie */

    if ((name = bksmt_dict_get(cookie, "Name")) == NULL)
        return COOKIE_ERROR;

    if ((val = bksmt_dict_get(cookie, "Value")) == NULL)
        return COOKIE_ERROR;

    xasprintf(ret, "%s=%s", name, val);

    /* loop through remaining attrs and append */
    BKSMT_DICT_FOREACH(cookie, e)
        if (strcmp(e->key, "Name") && strcmp(e->key, "Value"))
            xasprintf(ret, "%s; %s=%s", *ret, e->key, e->val);

    return COOKIE_OK;
}


