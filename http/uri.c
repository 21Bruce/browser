#include "uri.h"
#include <string.h>
#include <stdlib.h>
#include "../lib/xstring.h"
#include <stdio.h>


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
