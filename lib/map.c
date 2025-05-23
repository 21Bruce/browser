#include "map.h"

#include "hash.h"
#include "xmalloc.h"
#include "xstring.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/queue.h>

#define HASHINIT 100
#define HASHREFILL 0.8
#define HASHRESIZE 2
#define HASHCAP(map) ((float)((map)->nelem)/(float)((map)->nbuckets))

#define VCMP(map, val1, val2) ((map)->vcmp != NULL ? (map)->vcmp((val1), (val2)) : val1 != val2)

#define KCMP(map, key1, key2) ((map)->kcmp != NULL ? (map)->kcmp((key1), (key2)) : key1 != key2)

#define VCPY(map, val) (map)->vcpy != NULL ? (map)->vcpy((val)) : val

#define KCPY(map, key) (map)->kcpy != NULL ? (map)->kcpy((key)) : key

#define VFREE(map, val)       \
    if ((map)->vfree != NULL) \
        (map)->vfree((val))

#define KFREE(map, key)       \
    if ((map)->kfree != NULL) \
        (map)->kfree((key))


static void
bksmt_map_regrow(struct bksmt_map *map)
{
    struct bksmt_map_elem **nxtbuckets, *csrc, *cdst, *ctmp;
    unsigned long nbidx;
    size_t i, resize = (map->nbuckets) * HASHRESIZE;

    assert(map != NULL);

    /*
     * Loop through all elements, rehash them into new 
     * hash map, then free them
     */
    nxtbuckets = xzallocarray(resize, sizeof *nxtbuckets);
    for(i = 0; i < map->nbuckets; i++) {
        for(csrc = map->buckets[i]; csrc != NULL; csrc = ctmp) {
            nbidx = djb2_hash(csrc->key) % resize;
            cdst = xmalloc(sizeof *cdst);
            cdst->key = csrc->key;
            cdst->val = csrc->val;
            cdst->nxt = nxtbuckets[nbidx];
            nxtbuckets[nbidx] = cdst;
            ctmp = csrc->nxt; 
            free(csrc);
        }  
    }
    free(map->buckets);
    map->buckets = nxtbuckets;
    map->nbuckets = resize;
}

struct bksmt_map *
bksmt_map_init(int(*vcmp)(void*,void*), int(*kcmp)(void*,void*), void*(*vcpy)(void*), void*(*kcpy)(void*), unsigned long(*khash)(void*), void(*vfree)(void*), void(*kfree)(void*), void*(*vinit)(), int cvos)
{
    struct bksmt_map *ret = xmalloc(sizeof *ret);
    ret->vcmp = vcmp;
    ret->kcmp = kcmp;
    ret->vcpy = vcpy;
    ret->kcpy = kcpy;
    ret->khash = khash;
    ret->vfree = vfree;
    ret->kfree = kfree;
    ret->vinit = vinit;
    ret->nbuckets = HASHINIT;
    ret->buckets = xzallocarray(ret->nbuckets, sizeof *(ret->buckets));
    ret->nelem = 0;
    ret->cvos = cvos;
    LIST_INIT(&(ret->elems));
    return ret;
}

void *
bksmt_map_get(struct bksmt_map *map, void *key, int flags)
{
    struct bksmt_map_elem *c, *nc;
    unsigned long idx;

    assert(map != NULL);

    idx = map->khash(key) % map->nbuckets;

    for (c = map->buckets[idx]; c != NULL; c = c->nxt) {
        if (KCMP(map, key, c->key) == 0) {
            return c->val;
        }
    }

    if ((flags & BKSMT_MAP_NINIT) != 0 || map->vinit == NULL)
        return NULL;

    /* check for regrow */
    if (HASHCAP(map) >= HASHREFILL)
        bksmt_map_regrow(map);

    /* create new elem */
    nc = xmalloc(sizeof *nc);
    nc->key = KCPY(map, key);
    nc->val = map->vinit();
    nc->nxt = map->buckets[idx];
    map->buckets[idx] = nc;
    map->nelem += 1;
    LIST_INSERT_HEAD(&(map->elems), nc, elist);

    /* return fresh val */
    return nc->val;
}

void 
bksmt_map_set(struct bksmt_map *map, void *key, void *val)
{
    struct bksmt_map_elem *c, *nc;
    unsigned long bidx;
    
    assert(map != NULL);

    bidx = map->khash(key) % map->nbuckets;
    for (c = map->buckets[bidx]; c != NULL; c = c->nxt) {
        if (KCMP(map, key, c->key) == 0) {
            VFREE(map, c->val);
            c->val = map->cvos ? (VCPY(map, val)) : val; 
            return;
        }
    }

    if (HASHCAP(map) >= HASHREFILL) 
        bksmt_map_regrow(map);

    nc = xmalloc(sizeof *nc);
    nc->key = KCPY(map, key);     
    nc->val = map->cvos ? (VCPY(map, val)) : val; 
    nc->nxt = map->buckets[bidx];
    map->buckets[bidx] = nc;
    map->nelem += 1;
    LIST_INSERT_HEAD(&(map->elems), nc, elist);
}

void
bksmt_map_clear(struct bksmt_map *map, void *key)
{
    struct bksmt_map_elem *c, *p = NULL;
    unsigned long bidx;
 
    assert(map != NULL);
    bidx = map->khash(key) % map->nbuckets;
    for (c = map->buckets[bidx]; c != NULL && KCMP(map, key, c->key) != 0; c = c->nxt) {
        p = c;
    }

    if (c != NULL) {
        /* remove elem before freeing */
        if (p != NULL)
            p->nxt = c->nxt;
        else 
            map->buckets[bidx] = c->nxt;

        KFREE(map, c->key);
        VFREE(map, c->val);
        LIST_REMOVE(c, elist);
        free(c);
        map->nelem -= 1;
    }
}

void
bksmt_map_apply(struct bksmt_map *dst, struct bksmt_map *src)
{
    struct bksmt_map_elem *e;

    assert(dst != NULL && src != NULL);

    BKSMT_MAP_FOREACH(src, e)
        bksmt_map_set(dst, e->key, e->val);
}

void 
bksmt_map_print(struct bksmt_map *src, void(*print)(void*,void*))
{
    struct bksmt_map_elem *e;

    if (src == NULL)
        return; 

    BKSMT_MAP_FOREACH(src, e)
        print(e->key, e->val);
}

struct bksmt_map *
bksmt_map_cpy(struct bksmt_map *src)
{
    struct bksmt_map *dst;

    dst = bksmt_map_init(src->vcmp, src->kcmp, src->vcpy, src->kcpy, src->khash, src->vfree, src->kfree, src->vinit, src->cvos);
    bksmt_map_apply(dst, src);
    return dst;
}


/* BUG HERE */
int 
bksmt_map_eq(struct bksmt_map *src1, struct bksmt_map *src2)
{
    struct bksmt_map_elem *e1, *e2;
    int flag;

    assert(src1->kcmp == src2->kcmp && src1->vcmp == src2->vcmp);

    flag = 0;
    BKSMT_MAP_FOREACH(src1, e1) {
        BKSMT_MAP_FOREACH(src2, e2) {
            if (KCMP(src1, e1->key, e2->key) == 0 && VCMP(src1, e1->val, e2->val))
                flag = 1;
        }
        if (flag == 1)
            return 0;
        flag = 0;
    }
    return 1;
}

void
bksmt_map_free(struct bksmt_map *map)
{
    struct bksmt_map_elem *c, *ctmp;
    size_t i;

    assert(map != NULL);

    for (i = 0; i < map->nbuckets; i++) {
        for (c = map->buckets[i]; c != NULL; c = ctmp) {
            ctmp = c->nxt;
            VFREE(map, c->val);
            KFREE(map, c->key);
            free(c);
        }
    }
    free(map->buckets);
    free(map);
}


