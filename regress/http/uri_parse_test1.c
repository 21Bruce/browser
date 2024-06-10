#include "../../http/http.h"
#include "../../http/uri.h"
#include "../../lib/dict.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int check_uri(struct bksmt_uri *, int, char *, int, char *, struct bksmt_dict *, char *);

int 
main(void)
{
    /* decompose a full uri */
    int stat;
    char *uri = "https://google.com:63/absdf/adf?a=b&t=c#abc";
    int port = 63;
    int protocolk = HTTP_HTTPS;
    char *dn = "google.com";
    char *fpath = "/absdf/adf";
    char *anchor = "abc";
    struct bksmt_uri *sto;
    struct bksmt_dict *parameters;

    parameters = bksmt_dict_init();
    bksmt_dict_set(parameters,"a", "b"); 
    bksmt_dict_set(parameters,"t", "c"); 

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 1; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 1 + stat; 


    bksmt_uri_free(sto);
    bksmt_dict_free(parameters);

   
    /* decompose many partial uris */

    /* #1 no protocol, should assume http*/
    uri = "google.com:63/absdf/adf?a=b&t=c#abc";
    port = 63;
    protocolk = HTTP_HTTP;
    dn = "google.com";
    fpath = "/absdf/adf";
    anchor = "abc";
    parameters = bksmt_dict_init();
    bksmt_dict_set(parameters,"a", "b"); 
    bksmt_dict_set(parameters,"t", "c"); 

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 7; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 7 + stat; 

    bksmt_uri_free(sto);
    bksmt_dict_free(parameters);

    /* #2 no port, should assume 0*/
    uri = "google.com/absdf/adf?a=b&t=c#abc";
    port = 0;
    protocolk = HTTP_HTTP;
    dn = "google.com";
    fpath = "/absdf/adf";
    anchor = "abc";
    parameters = bksmt_dict_init();
    bksmt_dict_set(parameters,"a", "b"); 
    bksmt_dict_set(parameters,"t", "c"); 

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 13; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 13 + stat; 

    bksmt_uri_free(sto);
    bksmt_dict_free(parameters);

    /* #3 no fpath, should assume none */
    uri = "google.com?a=b&t=c#abc";
    port = 0;
    protocolk = HTTP_HTTP;
    dn = "google.com";
    fpath = NULL;
    anchor = "abc";
    parameters = bksmt_dict_init();
    bksmt_dict_set(parameters,"a", "b"); 
    bksmt_dict_set(parameters,"t", "c"); 

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 19; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 19 + stat; 

    bksmt_uri_free(sto);
    bksmt_dict_free(parameters);

    /* #4 no params, should assume none */
    uri = "google.com#abc";
    port = 0;
    protocolk = HTTP_HTTP;
    dn = "google.com";
    fpath = NULL;
    anchor = "abc";
    parameters = NULL;

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 25; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 25 + stat; 

    bksmt_uri_free(sto);

    /* #5 no anchor, should assume none */
    uri = "google.com";
    port = 0;
    protocolk = HTTP_HTTP;
    dn = "google.com";
    fpath = NULL;
    parameters = NULL;
    anchor = NULL;

    stat = bksmt_uri_parse(uri, &sto);
    if (stat != HTTP_OK)
       return 31; 

    stat = check_uri(sto, protocolk, dn, port, fpath, parameters, anchor);

    if (stat != 0)
       return 31 + stat; 

    bksmt_uri_free(sto);



    return 0;
}


static int 
check_uri(struct bksmt_uri *sto, int protocolk, char *dn, int port, char *fpath, struct bksmt_dict *parameters, char *anchor)
{
    if (sto->protocolk != protocolk)    
        return 1;

    if (sto->dn == NULL || strcmp(sto->dn, dn) != 0)
        return 2;

    if (sto->port != port)
        return 3;

    if (sto->fpath == NULL && fpath != NULL) 
        return 4;
    else if (fpath == NULL && sto->fpath != NULL)  
        return 4;
    else if (fpath != NULL && sto->fpath != NULL & strcmp(sto->fpath, fpath) != 0)
        return 4;

    if (sto->parameters == NULL && parameters != NULL) 
        return 5;
    else if (parameters == NULL && sto->parameters != NULL)  
        return 5;
    if (parameters != NULL && sto->parameters != NULL && !bksmt_dict_eq(sto->parameters, parameters))
        return 5;

    if (sto->anchor == NULL && anchor != NULL) 
        return 6;
    else if (anchor == NULL && sto->anchor != NULL)  
        return 6;
    else if (anchor != NULL && sto->anchor != NULL & strcmp(sto->anchor, anchor) != 0)
        return 6;

    return 0;
}
