#ifndef __BKSMT_MAP_H__
#define __BKSMT_MAP_H__

#include <stdlib.h>
#include <sys/queue.h>

struct bksmt_map_elem {
    LIST_ENTRY(bksmt_map_elem)    elist;
    void                         *key;
    void                         *val;
    struct bksmt_map_elem        *nxt;
};

struct bksmt_map {
    LIST_HEAD(, bksmt_map_elem)   elems; 
    size_t                        nbuckets;
    struct bksmt_map_elem       **buckets;  
    size_t                        nelem;
    int(*vcmp)(void*,void*); 
    int(*kcmp)(void*,void*); 
    void*(*vcpy)(void*); 
    void*(*kcpy)(void*);
    unsigned long(*khash)(void*);
    void(*vfree)(void*); 
    void(*kfree)(void*);
};

struct bksmt_map  *bksmt_map_init(int(*)(void*,void*), int(*)(void*,void*), void*(*)(void*), void*(*)(void*), unsigned long(*)(void*), void(*)(void*), void(*)(void*));
void              *bksmt_map_get(struct bksmt_map*, void*);
void               bksmt_map_set(struct bksmt_map*, void*, void*);
void               bksmt_map_apply(struct bksmt_map*, struct bksmt_map*);
int                bksmt_map_eq(struct bksmt_map *, struct bksmt_map *);
void               bksmt_map_clear(struct bksmt_map *, void *);
void               bksmt_map_print(struct bksmt_map *, void(*)(void*,void*));
void               bksmt_map_free(struct bksmt_map *);

#define BKSMT_MAP_FOREACH(map, e) \
    LIST_FOREACH(e, &((map)->elems), elist)

#endif
