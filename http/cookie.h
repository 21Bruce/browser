#ifndef __BKSMT_HTTP_COOKIE_H__
#define __BKSMT_HTTP_COOKIE_H__

#include "../lib/dict.h"
#include "../lib/dictcase.h"

/* helper for Set-Cookie parsing */
int parse_cookie(char * , struct bksmt_map **);

/* 
 * builds cookie string with all attributes. 
 * Incoming cookie dict must have 'Name' 
 * and 'Value' fields at a minimum.
 */
int build_cookie(struct bksmt_map *, char **);


/* cookie stat returns */

#define COOKIE_OK    1
#define COOKIE_ERROR 2

/* cookie build flags */

#endif /* __BKSMT_HTTP_COOKIE_H__ */

