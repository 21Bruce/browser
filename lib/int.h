#ifndef __BKSMT_LIB_INT_H__
#define __BKSMT_LIB_INT_H__

#include <stdint.h>
#include <stdlib.h>

#define BKSMT_INT_SL_MAX 18446744073709551615

/* unbounded int arithmetic functions interface */

/* representation of a big int */
struct bksmt_int {
    uint64_t *num;
    size_t size;
    int sign;
};

/* representation of a big uint */
struct bksmt_uint {
    uint64_t *num;
    size_t size;
};

/* dynamically initialize int with initial int */
struct bksmt_int *bksmt_int_init_int(int64_t);

/* dynamically initialize int with initial uint64 arr */
struct bksmt_int *bksmt_int_init_lst(uint64_t *, size_t, int);

/* dynamically initialize int with deep duplication of input int */
struct bksmt_int *bksmt_int_dup(struct bksmt_int *);

/* cmp two ints:
 *
 * if #1 > #2 --> output 1
 * if #1 = #2 --> output 0
 * if #1 < #2 --> output -1 
 *
 */ 
int bksmt_int_cmp(struct bksmt_int *, struct bksmt_int *);

/* add two ints, dynamically generated result */ 
struct bksmt_int *bksmt_int_add(struct bksmt_int *, struct bksmt_int *);

/* sub two ints, dynamically generated result */ 
struct bksmt_int *bksmt_int_sub(struct bksmt_int *, struct bksmt_int *);

struct bksmt_int *bksmt_int_mul(struct bksmt_int *, struct bksmt_int *);

struct bksmt_int *bksmt_int_div(struct bksmt_int *, struct bksmt_int *);

struct bksmt_int *bksmt_int_mod(struct bksmt_int *, struct bksmt_int *);

struct bksmt_int *bksmt_int_modmul(struct bksmt_int *, struct bksmt_int *, struct bksmt_int *);

/* in-place addition, 1st arg is dst, 2nd is src */
void bksmt_int_adds(struct bksmt_int *, struct bksmt_int *);

/* in-place subtraction, 1st arg is dst, 2nd is src */
void bksmt_int_subs(struct bksmt_int *, struct bksmt_int *);

/* in-place multiplication, 1st arg is dst, 2nd is src */
void bksmt_int_muls(struct bksmt_int *, struct bksmt_int *);

/* in-place division, 1st arg is dst, 2nd is src */
void bksmt_int_divs(struct bksmt_int *, struct bksmt_int *);

/* in-place modulo, 1st arg is dst, 2nd is src */
void bksmt_int_mods(struct bksmt_int *, struct bksmt_int *);

/* in-place modular multiplication, i1=(i1*i2)%i3 */
void bksmt_int_modmuls(struct bksmt_int *, struct bksmt_int *, struct bksmt_int *);

/* get an array of numbers representing int with a sign int */
void bksmt_int_lst_rep(struct bksmt_int *, uint64_t **, size_t *, int *);

/* free function */
void bksmt_int_free(struct bksmt_int *);


#endif /* __BKSMT_LIB_INT_H__ */
