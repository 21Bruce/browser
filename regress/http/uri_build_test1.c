#include "../../http/http.h"
#include "../../http/uri.h"
#include "../../lib/dict.h"
#include "../../lib/xmalloc.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int check_uri(struct bksmt_uri *, int, char *, int, char *, struct bksmt_dict *, char *);

int 
main(void)
{
    /* reconstruct a full uri */
    int stat;
    char *uri1 = "https://google.com:63/absdf/adf?t=c&a=b#abc";
    char *uri2 = "https://google.com:63/absdf/adf?a=b&t=c#abc";
    char *usto;
    int port = 63;
    int protocolk = HTTP_HTTPS;
    char *dn = "google.com";
    char *fpath = "/absdf/adf";
    char *anchor = "abc";
    struct bksmt_dict *parameters;
    struct bksmt_uri *ustct;

    ustct = xzalloc(sizeof *ustct);

    ustct->protocolk = protocolk;
    ustct->port = port;
    ustct->dn = dn;
    ustct->fpath = fpath;
    ustct->anchor = anchor;

    parameters = bksmt_dict_init();
    bksmt_dict_set(parameters,"a", "b"); 
    bksmt_dict_set(parameters,"t", "c"); 

    ustct->parameters = parameters;

    stat = bksmt_uri_build(ustct, &usto, 0);
    if (stat != HTTP_OK)
       return 1; 

    if (strcmp(uri1, usto) != 0 && strcmp(uri2, usto) != 0)
        return 2;

}
