#ifndef __BKSMT_DICT_H__
#define __BKSMT_DICT_H__

#include <stdlib.h>
#include <sys/queue.h>
#include <string.h>

#include "map.h"
#include "xstring.h"
#include "hash.h"

BKSMT_MAP_TYPE(dict, char, char)

#define bksmt_dict_init() \
    bksmt_map_init(strcmp, strcmp, xstrdup, xstrdup, djb2_hash, free, free)
#define bksmt_dict_get(dict, key) \
    bksmt_map_get((struct bksmt_map *)(dict), (void *)(key))
#define bksmt_dict_set(dict, key, val) \
    bksmt_map_set((struct bksmt_map *)(dict), (void *)(key), (void *)(val))
#define bksmt_dict_apply(dict1, dict2) \
    bksmt_map_apply((struct bksmt_map *)(dict1), (struct bksmt_map *)(dict2))
#define bksmt_dict_eq(dict1, dict2) \
    bksmt_map_eq((struct bksmt_map *)(dict1), (struct bksmt_map *)(dict2))
#define bksmt_dict_clear(dict, key) \
    bksmt_map_clear((struct bksmt_map *)(dict), (void *)(key))
#define bksmt_dict_print(dict, pf) \
    bksmt_map_print((struct bksmt_map *)(dict), (pf)) 
#define bksmt_dict_free(dict) \
    bksmt_map_free((struct bksmt_map *)(dict))

#define BKSMT_DICT_FOREACH(dict, e) \
    BKSMT_MAP_FOREACH((struct bksmt_map *)(dict), (e))

#endif
