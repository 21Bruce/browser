#include "dictcase.h"

#include <assert.h>
#include <string.h>

#include "dict.h"
#include "hash.h"
#include "xmalloc.h"
#include "xstring.h"

#define HASHINIT 100
#define HASHREFILL 0.8
#define HASHRESIZE 2
#define HASHCAP(dictcase) ((float)(dictcase->nelem)/(float)(dictcase->nbuckets))

static void bksmt_dictcase_regrow(struct bksmt_dictcase *);

static void
bksmt_dictcase_regrow(struct bksmt_dictcase *dictcase)
{
    struct bksmt_dictcase_elem **nxtbuckets, *csrc, *cdst, *ctmp;
    unsigned long nbidx;
    size_t i, resize;

    assert(dictcase != NULL);

    resize = (dictcase->nbuckets) * HASHRESIZE;

    /*
     * Loop through all elements, rehash them into new 
     * hash map, then free them
     */
    nxtbuckets = xzallocarray(resize, sizeof *nxtbuckets);
    for(i = 0; i < dictcase->nbuckets; i++) {
        for(csrc = dictcase->buckets[i]; csrc != NULL; csrc = ctmp) {
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
    free(dictcase->buckets);
    dictcase->buckets = nxtbuckets;
    dictcase->nbuckets = resize;
}

struct bksmt_dictcase *
bksmt_dictcase_init(void)
{
    struct bksmt_dictcase *ret = xmalloc(sizeof *ret);
    ret->nbuckets = HASHINIT;
    ret->buckets = xzallocarray(ret->nbuckets, sizeof *(ret->buckets));
    ret->nelem = 0;
    return ret;
}

struct bksmt_dict *
bksmt_dictcase_get(struct bksmt_dictcase *dictcase, char *key, int flags) 
{
    struct bksmt_dictcase_elem *c, *nc;
    unsigned long idx;

    assert(dictcase != NULL);

    /* find hash using djb2 */
    idx = djb2_hash(key) % dictcase->nbuckets;

    /* if there is a dict return it */
    for(c = dictcase->buckets[idx]; c != NULL; c = c->nxt) {
        if (strcmp(key, c->key) == 0)
            return c->val;
    }

    /* if flagged to not create, quit here */
    if (flags & BKSMT_DICTCASE_NCREAT)
        return NULL;

    /* check for regrow */
    if (HASHCAP(dictcase) >= HASHREFILL)
        bksmt_dictcase_regrow(dictcase);

    /* create new elem */
    nc = xmalloc(sizeof *nc);
    nc->key = xstrdup(key);
    nc->val = bksmt_dict_init();
    nc->nxt = dictcase->buckets[idx];
    dictcase->buckets[idx] = nc;
    dictcase->nelem += 1;

    /* return fresh dict */
    return nc->val;
}

void 
bksmt_dictcase_clear(struct bksmt_dictcase *dictcase, char *key)
{
    struct bksmt_dictcase_elem *c, *p = NULL;
    unsigned long bidx;
 
    assert(dictcase != NULL);

    bidx = djb2_hash(key) % dictcase->nbuckets;
    for (c = dictcase->buckets[bidx]; c != NULL && strcmp(key, c->key) != 0; c = c->nxt) 
        p = c;

    if (c != NULL) {
        /* remove elem before freeing */
        if (p != NULL)
            p->nxt = c->nxt;
        else 
            dictcase->buckets[bidx] = c->nxt;

        free(c->key);
        bksmt_dict_free(c->val);
        free(c);
        dictcase->nelem -= 1;
    }
}

void 
bksmt_dictcase_free(struct bksmt_dictcase *dictcase)
{
    struct bksmt_dictcase_elem *c, *ctmp;
    size_t i;

    assert(dictcase != NULL);

    for (i = 0; i < dictcase->nbuckets; i++) {
        for (c = dictcase->buckets[i]; c != NULL; c = ctmp) {
            ctmp = c->nxt;
            free(c->key);
            bksmt_dict_free(c->val);
            free(c);
        }
    }
    free(dictcase->buckets);
    free(dictcase);

}
