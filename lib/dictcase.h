#ifndef __BKSMT_LIB_DICTCASE_H__
#define __BKSMT_LIB_DICTCASE_H__

/*
 * A dictcase is a map of string keys to dictionary vals, 
 * basically a data structure that labels dictionaries
 */

#include <stdlib.h>
#include <sys/queue.h>

#include "dict.h"
#include "map.h"

BKSMT_MAP_TYPE(dictcase, char, struct bksmt_dict)

#define bksmt_dictcase_init() \
    bksmt_map_init(bksmt_map_eq, strcmp, bksmt_map_cpy, xstrdup, djb2_hash, bksmt_map_free, free, bksmt_dict_vinit, 0)

struct bksmt_dictcase *bksmt_dictcase_vinit(void);

#define bksmt_dictcase_get(dc, key, flag) \
    bksmt_map_get((struct bksmt_map *)(dc), (void *)(key), flag)
#define bksmt_dictcase_set(dc, key, val) \
    bksmt_map_set((struct bksmt_map *)(dc), (void *)(key), (void *)(val))
#define bksmt_dictcase_apply(dc1, dc2) \
    bksmt_map_apply((struct bksmt_map *)(dc1), (struct bksmt_map *)(dc2))
#define bksmt_dictcase_eq(dc1, dc2) \
    bksmt_map_eq((struct bksmt_map *)(dc1), (struct bksmt_map *)(dc2))
#define bksmt_dictcase_clear(dc, key) \
    bksmt_map_clear((struct bksmt_map *)(dc), (void *)(key))
#define bksmt_dictcase_print(dc, pf) \
    bksmt_map_print((struct bksmt_map *)(dc), (pf)) 
#define bksmt_dictcase_free(dc) \
    bksmt_map_free((struct bksmt_map *)(dc))

#define BKSMT_DICTCASE_FOREACH(dc, e) \
    BKSMT_MAP_FOREACH((struct bksmt_map *)(dc), (e))

/* get flags */

/* do not create if dc is not found */
#define BKSMT_DICTCASE_NCREAT BKSMT_MAP_NINIT 

#endif /* __BKSMT_LIB_DICTCASE_H__ */
