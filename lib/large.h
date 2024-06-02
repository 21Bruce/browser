#ifndef __BKSMT_LIB_LARGE_H__
#define __BKSMT_LIB_LARGE_H__

#include <stdint.h>
#include <stdlib.h>

/* large number arithmetic functions interface */

/* 
 * name conventions: 
 *
 *  *s - operation * that stores result in first arg
 */

/* representation of a big int */
struct bksmt_bigint {
    uint64_t *num;
    size_t size;
    int sign;
};

/* representation of a big uint */
struct bksmt_biguint {
    uint64_t *num;
    size_t size;
};

/* dynamically initialize bigint with initial int */
struct bksmt_bigint *bksmt_bigint_init_int(int);

/* dynamically initialize bigint with initial uint64 arr */
struct bksmt_bigint *bksmt_bigint_init_lst(uint64_t *, size_t, int);

/* dynamically initialize bigint with deep duplication of input bigint */
struct bksmt_bigint *bksmt_bigint_dup(struct bksmt_bigint *);

/* add two bigints, dynamically generated result */ 
struct bksmt_bigint *bksmt_bigint_add(struct bksmt_bigint *, struct bksmt_bigint *);

/* in-place addition, 1st arg is dst, 2nd is src */
void bksmt_bigint_adds(struct bksmt_bigint *, struct bksmt_bigint *);

/* get an array of numbers representing bigint with a sign int */
void bksmt_bigint_lst_rep(struct bksmt_bigint *src, uint64_t **num, size_t *size, int *sign);


#endif /* __BKSMT_LIB_LARGE_H__ */
