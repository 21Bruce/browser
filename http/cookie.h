#ifndef __BKSMT_HTTP_COOKIE_H__
#define __BKSMT_HTTP_COOKIE_H__

#include "../lib/dict.h"
#include "../lib/dictcase.h"

/* helper for Set-Cookie parsing */
int parse_cookie(struct bksmt_dictcase *, char *);

/* builds cookie string with all attributes */
int build_cookie(struct bksmt_dict *, char *, char **);

/* this is for the 'Cookie' header, use build_cookie and a foreach for 'Set-Cookie' */
int build_cookies(struct bksmt_dictcase *, char **);

/* cookie stat returns */

#define COOKIE_OK    1
#define COOKIE_ERROR 2

#endif /* __BKSMT_HTTP_COOKIE_H__ */

