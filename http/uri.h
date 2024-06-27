#ifndef __BKSMT_HTTP_URI_H__
#define __BKSMT_HTTP_URI_H__

#include "../lib/dict.h"

struct bksmt_uri {
    int                protocolk;
    char              *dn;
    char              *fpath;
    int                port;
    struct bksmt_map  *parameters;
    char              *anchor;
};

char *bksmt_cstrpctenc(char *);

int bksmt_uri_parse(char *, struct bksmt_uri **);

int bksmt_uri_build(struct bksmt_uri *, char **, int);

int bksmt_authority_extract(char *, char **);

void bksmt_uri_clear(struct bksmt_uri *);

void bksmt_uri_free(struct bksmt_uri *);

/* uri build flags */

/* apply a pct encoding to parameters */
#define BKSMT_URI_PARSE_PCTENC 0x1

#endif
