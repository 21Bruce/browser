#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "http/cookie.h"
#include "http/client.h"
#include "http/response.h"
#include "http/request.h"
#include "lib/dictcase.h"
#include "lib/dict.h"
#include "lib/bufread.h"

int 
main(int argc, char *argv[])
{
    struct bksmt_http_req *req;
    struct bksmt_http_res *res;
    struct bksmt_http_client *client;
    struct bksmt_http_status_lut_entry le;
    struct bksmt_dict_elem *de;
    struct bksmt_dictcase_elem *dce;
    char *val;
    char out[4096];
    int stat, i, size;

    if (argc < 2) {
        fprintf(stderr, "Usage: ./booksmart uri\n");
        return -1;
    }

    stat = bksmt_http_req_init(argv[1], HTTP_GET, NULL, 0, 0, &req);
    if (stat == HTTP_ERROR){
        fprintf(stderr, "error: improper uri\n");
        return -1;
    }

    bksmt_dict_set(req->header.mfields, "Accept-Language", "de"); 

    client = bksmt_http_client_init();

    stat = bksmt_http_client_do(client, req, 0, &res);
    if (stat == HTTP_ERROR){
        fprintf(stderr, "error: invalid message or network error\n");
        return -1;
    }

    le = bksmt_http_status_lut[res->header.statk];
    printf("HTTP/%d.%d %s %s\n", res->header.vmajor, 
            res->header.vminor, le.code, le.stat_msg);

    if (res->header.mfields)
        BKSMT_DICT_FOREACH(res->header.mfields, de)
            printf("%s: %s\n", de->key, de->val);

    if (res->header.cookies)
        BKSMT_DICTCASE_FOREACH(res->header.cookies, dce) {
            val = bksmt_dict_get(dce->val, "Value");
            printf("Set-Cookie: %s=%s", dce->key, val);
            BKSMT_DICT_FOREACH(dce->val, de) {
                if (strcmp(de->key, "Name") && strcmp(de->key, "Value"))
                    printf("; %s=%s", de->key, de->val);
            }
            printf("\n");
        }

    if (res->body) {
        stat = BKSMT_BUFREAD_OK;
        while (stat == BKSMT_BUFREAD_OK) {
            size = 4096;
            stat = bksmt_bufread_read(res->body, out, 0, &size);
            for(i = 0; i < size; i++)
                printf("%c", (char)out[i]); 
        }
    }

    return 0;
}
