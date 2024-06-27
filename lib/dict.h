#ifndef __BKSMT_DICT_H__
#define __BKSMT_DICT_H__

#include <stdlib.h>
#include <sys/queue.h>
#include <string.h>

#include "map.h"
#include "xstring.h"
#include "hash.h"

#define bksmt_dict_init() \
    bksmt_map_init(strcmp, strcmp, xstrdup, xstrdup, djb2_hash, free, free)
#define bksmt_dict_get(dict, key) \
    bksmt_map_get((dict), (void *)(key))
#define bksmt_dict_set(dict, key, val) \
    bksmt_map_set((dict), (void *)(key), (void *)(val))
#define bksmt_dict_apply(dict1, dict2) \
    bksmt_map_apply((dict1), (dict2))
#define bksmt_dict_eq(dict1, dict2) \
    bksmt_map_eq((dict1), (dict2))
#define bksmt_dict_clear(dict, key) \
    bksmt_map_clear((dict), (void *)(key))
#define bksmt_dict_print(dict, pf) \
    bksmt_map_print((dict), (pf)) 
#define bksmt_dict_free(dict) \
    bksmt_map_free((dict))

#define BKSMT_DICT_FOREACH(dict, e) \
    BKSMT_MAP_FOREACH((dict), (e))

#endif
