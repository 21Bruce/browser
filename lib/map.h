#ifndef __BKSMT_MAP_H__
#define __BKSMT_MAP_H__

#include <stdlib.h>
#include <sys/queue.h>

#define BKSMT_MAP_TYPE(type, ktype, vtype)               \
    struct bksmt_ ## type ## _elem {                     \
        LIST_ENTRY(bksmt_ ## type ## _elem)    elist;    \
        ktype                         *key;              \
        vtype                         *val;              \
        struct bksmt_ ## type ## _elem   *nxt;           \
    };                                                   \
                                                         \
    struct bksmt_ ## type {                              \
        LIST_HEAD(, bksmt_ ## type ## _elem)   elems;    \
        size_t                                 nbuckets; \
        struct bksmt_ ## type ## _elem       **buckets;  \
        size_t                                 nelem;    \
        int(*vcmp)(vtype *, vtype *);                    \
        int(*kcmp)(ktype *, ktype *);                    \
        void*(*vcpy)(vtype *);                           \
        void*(*kcpy)(ktype *);                           \
        unsigned long(*khash)(ktype *);                  \
        void(*vfree)(vtype *);                           \
        void(*kfree)(ktype *);                           \
    };                                               


BKSMT_MAP_TYPE(map, void, void)


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
