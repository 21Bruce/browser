#include "dict.h"

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
#define HASHCAP(dict) ((float)(dict->nelem)/(float)(dict->nbuckets))

static void bksmt_dict_regrow(struct bksmt_dict *);

static void
bksmt_dict_regrow(struct bksmt_dict *dict)
{
    struct bksmt_dict_elem **nxtbuckets, *csrc, *cdst, *ctmp;
    unsigned long nbidx;
    size_t i, resize = (dict->nbuckets) * HASHRESIZE;

    assert(dict != NULL);

    /*
     * Loop through all elements, rehash them into new 
     * hash map, then free them
     */
    nxtbuckets = xzallocarray(resize, sizeof *nxtbuckets);
    for(i = 0; i < dict->nbuckets; i++) {
        for(csrc = dict->buckets[i]; csrc != NULL; csrc = ctmp) {
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
    free(dict->buckets);
    dict->buckets = nxtbuckets;
    dict->nbuckets = resize;
}

struct bksmt_dict *
bksmt_dict_init(void)
{
    struct bksmt_dict *ret = xmalloc(sizeof *ret);
    ret->nbuckets = HASHINIT;
    ret->buckets = xzallocarray(ret->nbuckets, sizeof *(ret->buckets));
    ret->nelem = 0;
    LIST_INIT(&(ret->elems));
    return ret;
}

char *
bksmt_dict_get(struct bksmt_dict *dict, char *key)
{
    struct bksmt_dict_elem *c;
    unsigned long idx;

    assert(dict != NULL);

    idx = djb2_hash(key) % dict->nbuckets;

    for (c = dict->buckets[idx]; c != NULL; c = c->nxt) {
        if (strcmp(key, c->key) == 0)
            return c->val;
    }

    return NULL;
}

void 
bksmt_dict_set(struct bksmt_dict *dict, char *key, char *val)
{
    struct bksmt_dict_elem *c, *nc;
    unsigned long bidx;
    
    assert(dict != NULL);

    if (HASHCAP(dict) >= HASHREFILL) 
        bksmt_dict_regrow(dict);

    bidx = djb2_hash(key) % dict->nbuckets;
    for (c = dict->buckets[bidx]; c != NULL; c = c->nxt) {
        if (strcmp(key, c->key) == 0) {
            free(c->val);
            c->val = xstrdup(val);
            return;
        }
    }
    nc = xmalloc(sizeof *nc);
    nc->key = xstrdup(key);
    nc->val = xstrdup(val);
    nc->nxt = dict->buckets[bidx];
    dict->buckets[bidx] = nc;
    dict->nelem += 1;
    LIST_INSERT_HEAD(&(dict->elems), nc, elist);
}

void
bksmt_dict_clear(struct bksmt_dict *dict, char *key)
{
    struct bksmt_dict_elem *c, *p = NULL;
    unsigned long bidx;
 
    assert(dict != NULL);
    bidx = djb2_hash(key) % dict->nbuckets;
    for (c = dict->buckets[bidx]; c != NULL && strcmp(key, c->key) != 0; c = c->nxt) {
        p = c;
    }

    if (c != NULL) {
        /* remove elem before freeing */
        if (p != NULL)
            p->nxt = c->nxt;
        else 
            dict->buckets[bidx] = c->nxt;

        free(c->key);
        free(c->val);
        LIST_REMOVE(c, elist);
        free(c);
        dict->nelem -= 1;
    }
}

void
bksmt_dict_apply(struct bksmt_dict *dst, struct bksmt_dict *src)
{
    struct bksmt_dict_elem *e;

    assert(dst != NULL && src != NULL);

    BKSMT_DICT_FOREACH(src, e)
        bksmt_dict_set(dst, e->key, e->val);
}

void 
bksmt_dict_print(struct bksmt_dict *src)
{
    struct bksmt_dict_elem *e;

    if (src == NULL)
        return; 

    BKSMT_DICT_FOREACH(src, e)
        fprintf(stderr, "%s: %s\n", e->key, e->val);
}


int 
bksmt_dict_eq(struct bksmt_dict *src1, struct bksmt_dict *src2)
{
    struct bksmt_dict_elem *e1, *e2;
    int flag;

    flag = 0;
    BKSMT_DICT_FOREACH(src1, e1) {
        BKSMT_DICT_FOREACH(src2, e2) {
            if (strcmp(e1->key, e2->key) == 0 && strcmp(e1->val, e2->val))
                flag = 1;
        }
        if (flag == 1)
            return 0;
        flag = 0;
    }
    return 1;
}

void
bksmt_dict_free(struct bksmt_dict *dict)
{
    struct bksmt_dict_elem *c, *ctmp;
    size_t i;

    assert(dict != NULL);

    for (i = 0; i < dict->nbuckets; i++) {
        for (c = dict->buckets[i]; c != NULL; c = ctmp) {
            ctmp = c->nxt;
            free(c->val);
            free(c->key);
            free(c);
        }
    }
    free(dict->buckets);
    free(dict);
}


