#include "cookie.h"

#include "../lib/dict.h"
#include "../lib/dictcase.h"
#include "../lib/xmalloc.h"
#include "../lib/xstring.h"

#include <string.h>
#include <assert.h>

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
parse_cookie(struct bksmt_dictcase *cstore, char *src)
{
    char *nxteq, *nxtscol, *tmpk, *tmpv;
    struct bksmt_dict *cookie;

    assert(src != NULL);
    assert(cstore != NULL);

    /* init cookie dict to null */
    cookie = NULL;

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

        /* if nxtcol is at src, no val, so err */
        if (nxtscol == src)
            goto verror;

        /* if no next semicol, cpy to end. else, cpy to semicol */
        if (nxtscol == NULL) 
            xasprintf(&tmpv, "%s", src);
        else 
            xasprintf(&tmpv, "%.*s", nxtscol - src, src);

        /* if we haven't initted cookie dict, use this k-v pair */
        if (cookie == NULL) {
            cookie = bksmt_dictcase_get(cstore, tmpk, 0);
            bksmt_dict_set(cookie, "Name", tmpk);
            bksmt_dict_set(cookie, "Value", tmpv);
        } else {
        /* otherwise, set k-v for this cookie */
            bksmt_dict_set(cookie, tmpk, tmpv);
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

    return COOKIE_OK;

verror:
    free(tmpk);
kerror:
    /* error getting key */

    /* destory cookie if initted */
    if (cookie != NULL) { 
        tmpk = bksmt_dict_get(cookie, "Name");
        bksmt_dictcase_clear(cstore, tmpk);
    }
    return COOKIE_ERROR;
}
