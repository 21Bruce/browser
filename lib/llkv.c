#include "llkv.h"

#include <assert.h>
#include <string.h>

#include "dictcase.h"
#include "hash.h"
#include "xmalloc.h"
#include "xstring.h"

#define HASHINIT 100
#define HASHREFILL 0.8
#define HASHRESIZE 2
#define HASHCAP(llkv) ((float)(llkv->nelem)/(float)(llkv->nbuckets))

static void bksmt_llkv_regrow(struct bksmt_llkv *);

static void
bksmt_llkv_regrow(struct bksmt_llkv *llkv)
{
    struct bksmt_llkv_elem **nxtbuckets, *csrc, *cdst, *ctmp;
    unsigned long nbidx;
    size_t i, resize;

    assert(llkv != NULL);

    resize = (llkv->nbuckets) * HASHRESIZE;

    /*
     * Loop through all elements, rehash them into new 
     * hash map, then free them
     */
    nxtbuckets = xzallocarray(resize, sizeof *nxtbuckets);
    for(i = 0; i < llkv->nbuckets; i++) {
        for(csrc = llkv->buckets[i]; csrc != NULL; csrc = ctmp) {
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
    free(llkv->buckets);
    llkv->buckets = nxtbuckets;
    llkv->nbuckets = resize;
}

struct bksmt_llkv *
bksmt_llkv_init(void)
{
    struct bksmt_llkv *ret = xmalloc(sizeof *ret);
    ret->nbuckets = HASHINIT;
    ret->buckets = xzallocarray(ret->nbuckets, sizeof *(ret->buckets));
    ret->nelem = 0;
    return ret;
}

struct bksmt_dictcase *
bksmt_llkv_get(struct bksmt_llkv *llkv, char *key, int flags) 
{
    struct bksmt_llkv_elem *c, *nc;
    unsigned long idx;

    assert(llkv != NULL);

    /* find hash using djb2 */
    idx = djb2_hash(key) % llkv->nbuckets;

    /* if there is a dict return it */
    for(c = llkv->buckets[idx]; c != NULL; c = c->nxt) {
        if (strcmp(key, c->key) == 0)
            return c->val;
    }

    /* if flagged to not create, quit here */
    if (flags & BKSMT_LLKV_NCREAT)
        return NULL;

    /* check for regrow */
    if (HASHCAP(llkv) >= HASHREFILL)
        bksmt_llkv_regrow(llkv);

    /* create new elem */
    nc = xmalloc(sizeof *nc);
    nc->key = xstrdup(key);
    nc->val = bksmt_dictcase_init();
    nc->nxt = llkv->buckets[idx];
    llkv->buckets[idx] = nc;
    llkv->nelem += 1;

    /* return fresh dictcase */
    return nc->val;
}

void 
bksmt_llkv_clear(struct bksmt_llkv *llkv, char *key)
{
    struct bksmt_llkv_elem *c, *p = NULL;
    unsigned long bidx;
 
    assert(llkv != NULL);

    bidx = djb2_hash(key) % llkv->nbuckets;
    for (c = llkv->buckets[bidx]; c != NULL && strcmp(key, c->key) != 0; c = c->nxt) 
        p = c;

    if (c != NULL) {
        /* remove elem before freeing */
        if (p != NULL)
            p->nxt = c->nxt;
        else 
            llkv->buckets[bidx] = c->nxt;

        free(c->key);
        bksmt_dictcase_free(c->val);
        free(c);
        llkv->nelem -= 1;
    }
}

void 
bksmt_llkv_free(struct bksmt_llkv *llkv)
{
    struct bksmt_llkv_elem *c, *ctmp;
    size_t i;

    assert(llkv != NULL);

    for (i = 0; i < llkv->nbuckets; i++) {
        for (c = llkv->buckets[i]; c != NULL; c = ctmp) {
            ctmp = c->nxt;
            free(c->key);
            bksmt_dictcase_free(c->val);
            free(c);
        }
    }
    free(llkv->buckets);
    free(llkv);

}
