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
#include "map.h"

BKSMT_MAP_TYPE(llkv, char, struct bksmt_dictcase)

#define bksmt_llkv_init() \
    bksmt_map_init(bksmt_map_eq, strcmp, bksmt_map_cpy, xstrdup, djb2_hash, bksmt_map_free, free, bksmt_dictcase_vinit, 0)

struct bksmt_llkv *bksmt_llkv_vinit(void);

#define bksmt_llkv_get(lv, key, flag) \
    bksmt_map_get((struct bksmt_map *)(lv), (void *)(key), flag)
#define bksmt_llkv_set(lv, key, val) \
    bksmt_map_set((struct bksmt_map *)(lv), (void *)(key), (void *)(val))
#define bksmt_llkv_apply(lv1, lv2) \
    bksmt_map_apply((struct bksmt_map *)(lv1), (struct bksmt_map *)(lv2))
#define bksmt_llkv_eq(lv1, lv2) \
    bksmt_map_eq((struct bksmt_map *)(lv1), (struct bksmt_map *)(lv2))
#define bksmt_llkv_clear(lv, key) \
    bksmt_map_clear((struct bksmt_map *)(lv), (void *)(key))
#define bksmt_llkv_print(lv, pf) \
    bksmt_map_print((struct bksmt_map *)(lv), (pf)) 
#define bksmt_llkv_free(lv) \
    bksmt_map_free((struct bksmt_map *)(lv))

#define BKSMT_LLKV_FOREACH(lv, e) \
    BKSMT_MAP_FOREACH((struct bksmt_map *)(lv), (e))

/* get flags */

/* do not create if dc is not found */
#define BKSMT_LLKV_NCREAT BKSMT_MAP_NINIT 

#endif /* __BKSMT_LIB_LLKV_H__ */
