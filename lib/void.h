#ifndef __BKSMT_VOID_H__
#define __BKSMT_VOID_H__

/* This component provides type safe conversions of various common
 * functions' arguments to void * arguments. Need this for some 
 * generic code manipulations
 */

#include "map.h"

int bksmt_void_map_eq(void *, void *);

void bksmt_void_map_free(void *);

int bksmt_void_strcmp(void *, void *);

void *bksmt_void_map_cpy(void *);

void *bksmt_void_xstrdup(void *);

unsigned long bksmt_void_djb2_hash(void *);

unsigned long bksmt_void_sdbm_hash(void *);


#endif /* __BKSMT_VOID_H__ */


