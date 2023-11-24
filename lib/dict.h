#ifndef __BKSMT_DICT_H__
#define __BKSMT_DICT_H__

#include <stdlib.h>
#include <sys/queue.h>

struct bksmt_dict_elem {
    LIST_ENTRY(bksmt_dict_elem)  elist;
    char                         *key;
    char                         *val;
    struct bksmt_dict_elem       *nxt;
};

struct bksmt_dict {
    LIST_HEAD(, bksmt_dict_elem)  elems; 
    size_t                        nbuckets;
    struct bksmt_dict_elem      **buckets;  
    size_t                        nelem;
};

struct bksmt_dict *bksmt_dict_init(void);
char              *bksmt_dict_get(struct bksmt_dict *, char *);
void               bksmt_dict_set(struct bksmt_dict *, char *, char *);
void               bksmt_dict_clear(struct bksmt_dict *, char *);
void               bksmt_dict_free(struct bksmt_dict *);

#define BKSMT_DICT_FOREACH(dict, e) \
    LIST_FOREACH(e, &(dict->elems), elist)

#endif
