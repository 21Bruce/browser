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

struct bksmt_uri *bksmt_uri_init(void);

int bksmt_uri_parse(struct bksmt_uri **, char *, int);

int bksmt_uri_build(struct bksmt_uri *, char **);

void bksmt_uri_free(struct bksmt_uri *);

/* uri parse flags */

/* apply a pct encoding to parameters */
#define BKSMT_URI_PARSE_PCTENC 0x1

/* verify input str follows official uri structure */
#define BKSMT_URI_PARSE_STRICT 0x2


#endif
