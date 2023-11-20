#ifndef __BKSMT_DICT_H__
#define __BKSMT_DICT_H__

struct bksmt_dict;

struct bksmt_dict *bksmt_dict_init();
char              *bksmt_dict_get(struct bksmt_dict *, char *);
void               bksmt_dict_set(struct bksmt_dict *, char *, char *);
void               bksmt_dict_free(struct bksmt_dict *);

#endif
