#include "dict.h"
#include "hash.h"
#include "xmalloc.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HASHINIT 100
#define HASHREFILL 0.8
#define HASHRESIZE 2
#define HASHCAP(dict) ((float)(dict->nelem)/(float)(dict->nbuckets))

struct bksmt_dict_elem {
    char                    *key;
    char                    *val;
    struct bksmt_dict_elem  *nxt;
};

struct bksmt_dict {
    size_t                   nbuckets;
    struct bksmt_dict_elem **buckets;  
    size_t                   nelem;
};

static void bksmt_dict_regrow(struct bksmt_dict *);

static void
bksmt_dict_regrow(struct bksmt_dict *dict)
{
    struct bksmt_dict_elem **nxtbuckets, *csrc, *cdst, *ctmp;
    unsigned long nbidx;
    size_t i, resize = (dict->nbuckets) * HASHRESIZE;

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
bksmt_dict_init()
{
    struct bksmt_dict *ret = xmalloc(sizeof *ret);
    ret->nbuckets = HASHINIT;
    ret->buckets = xzallocarray(ret->nbuckets, sizeof *(ret->buckets));
    ret->nelem = 0;
    return ret;
}

char *
bksmt_dict_get(struct bksmt_dict *dict, char *key)
{
    struct bksmt_dict_elem *c;
    unsigned long idx = djb2_hash(key) % dict->nbuckets;

    assert(dict != NULL);

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
            c->val = strdup(val);
            return;
        }
    }
    nc = xmalloc(sizeof *nc);
    nc->key = strdup(key);
    nc->val = strdup(val);
    nc->nxt = dict->buckets[bidx];
    dict->buckets[bidx] = nc;
}

void
bksmt_dict_free(struct bksmt_dict *dict)
{
    struct bksmt_dict_elem *c, *ctmp;
    size_t i;
    for (i = 0; i < dict->nbuckets; i++) {
        for (c = dict->buckets[i]; c != NULL; c = ctmp) {
            ctmp = c->nxt;
            free(c);
        }
    }
    free(dict->buckets);
    free(dict);
}
