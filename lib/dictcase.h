#ifndef __BKSMT_LIB_DICTCASE_H__
#define __BKSMT_LIB_DICTCASE_H__

/*
 * A dictcase is a map of string keys to dictionary vals, 
 * basically a data structure that labels dictionaries
 */

#include <stdlib.h>
#include <sys/queue.h>

#include "dict.h"

struct bksmt_dictcase_elem {
    LIST_ENTRY(bksmt_dictcase_elem)  elist;
    char                            *key;
    struct bksmt_map                *val;
    struct bksmt_dictcase_elem      *nxt;
};

struct bksmt_dictcase {
    LIST_HEAD(, bksmt_dictcase_elem)  elems;
    size_t                            nbuckets;
    struct bksmt_dictcase_elem      **buckets;
    size_t                            nelem;
};

struct bksmt_dictcase *bksmt_dictcase_init(void);
struct bksmt_map      *bksmt_dictcase_get(struct bksmt_dictcase *, char *, int);
void                   bksmt_dictcase_set(struct bksmt_dictcase *, char *, struct bksmt_map *);
void                   bksmt_dictcase_apply(struct bksmt_dictcase *, struct bksmt_dictcase *);
void                   bksmt_dictcase_clear(struct bksmt_dictcase *, char *);
void                   bksmt_dictcase_free(struct bksmt_dictcase *);

/* get flags */

/* do not create if dict is not found */
#define BKSMT_DICTCASE_NCREAT 0x1

#define BKSMT_DICTCASE_FOREACH(dictcase, e) \
    LIST_FOREACH(e, &((dictcase)->elems), elist)

#endif /* __BKSMT_LIB_DICTCASE_H__ */
