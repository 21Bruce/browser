#ifndef __BKSMT_LIB_LLKV_H__
#define __BKSMT_LIB_LLKV_H__

/*
 * A labelled labelled key value store (llkv)
 * is a map of string keys to dictcase vals, 
 * basically a datastructure that labels dictcases.
 * This structure mimics what we need for an in-memory
 * cookie jar 
 */

#include <stdlib.h>

#include "dictcase.h"

struct bksmt_llkv_elem {
    char                    *key;
    struct bksmt_dictcase   *val;
    struct bksmt_llkv_elem  *nxt;
};

struct bksmt_llkv {
    size_t                       nbuckets;
    struct bksmt_llkv_elem     **buckets;
    size_t                       nelem;
};

struct bksmt_llkv     *bksmt_llkv_init(void);
struct bksmt_dictcase *bksmt_llkv_get(struct bksmt_llkv *, char *, int);
void                   bksmt_llkv_clear(struct bksmt_llkv *, char *);
void                   bksmt_llkv_free(struct bksmt_llkv *);

/* get flags */

/* do not create if dictcase is not found */
#define BKSMT_LLKV_NCREAT 0x1

#endif /* __BKSMT_LIB_LLKV_H__ */
