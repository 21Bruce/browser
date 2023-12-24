#ifndef __BKSMT_LIB_DICTCASE_H__
#define __BKSMT_LIB_DICTCASE_H__

/*
 * A dictcase is a map of string keys to dictionary vals, 
 * basically a data structure that labels dictionaries
 */

#include <stdlib.h>

#include "dict.h"

struct bksmt_dictcase_elem {
    char                       *key;
    struct bksmt_dict          *val;
    struct bksmt_dictcase_elem *nxt;
};

struct bksmt_dictcase {
    size_t                        nbuckets;
    struct bksmt_dictcase_elem  **buckets;
    size_t                        nelem;
};

struct bksmt_dictcase *bksmt_dictcase_init(void);
struct bksmt_dict     *bksmt_dictcase_get(struct bksmt_dictcase *, char *, int);
void                   bksmt_dictcase_set(struct bksmt_dictcase *, char *, struct bksmt_dict *);
void                   bksmt_dictcase_clear(struct bksmt_dictcase *, char *);
void                   bksmt_dictcase_free(struct bksmt_dictcase *);

/* get flags */

/* do not create if dict is not found */
#define BKSMT_DICTCASE_NCREAT 0x1

#endif /* __BKSMT_LIB_DICTCASE_H__ */
