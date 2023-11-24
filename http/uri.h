#ifndef __BKSMT_HTTP_URI_H__
#define __BKSMT_HTTP_URI_H__

#include "../lib/dict.h"

struct bksmt_uri {
    int                protocolk;
    char              *dn;
    char              *fpath;
    int                port;
    struct bksmt_dict *parameters;
    char              *anchor;
};

char *bksmt_cstrpctenc(char *);

#endif
