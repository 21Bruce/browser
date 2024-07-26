#include <string.h>

#include "xstring.h" 
#include "hash.h" 
#include "map.h" 
#include "void.h"

int 
bksmt_void_map_eq(void *map1, void *map2)
{
    return bksmt_map_eq((struct bksmt_map *) map1, (struct bksmt_map *) map2);
}

void * 
bksmt_void_map_cpy(void *s1)
{
    return bksmt_map_cpy((struct bksmt_map *)s1);
}

void  
bksmt_void_map_free(void *map)
{
    return bksmt_map_free((struct bksmt_map *)map);
}

int 
bksmt_void_strcmp(void *s1, void *s2)
{
    return strcmp((char *)s1, (char *)s2);
}

void * 
bksmt_void_xstrdup(void *s1)
{
    return xstrdup((char *)s1);
}

unsigned long 
bksmt_void_djb2_hash(void *a) 
{
    return djb2_hash((unsigned char*)a);
}

unsigned long 
bksmt_void_sdbm_hash(void *a) 
{
    return sdbm_hash((unsigned char*)a);
}

