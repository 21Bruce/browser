#include "large.h"
#include "math.h"
#include "xmalloc.h"


#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UINT64_MAX ULONG_LONG_MAX

static void expand_num(struct bksmt_bigint *, size_t);
static void adds(struct bksmt_bigint *, struct bksmt_bigint *);
static void subs(struct bksmt_bigint *, struct bksmt_bigint *);
static int geq(struct bksmt_bigint *, struct bksmt_bigint *);

struct bksmt_bigint *
bksmt_bigint_init_int(int i)
{
    struct bksmt_bigint *ret;

    ret = xzalloc(sizeof *ret);
    ret->num = xzalloc(sizeof *(ret->num));
    ret->size = 1;
    if (i < 0) {
        ret->num[0] = -i;
        ret->sign = 1;
    } else {
        ret->num[0] = i;
        ret->sign = 0;
    }

    return ret;
}

struct bksmt_bigint *
bksmt_bigint_init_lst(uint64_t *num, size_t size, int sign)
{
    struct bksmt_bigint *ret;
    
    ret = xzalloc(sizeof *ret);
    ret->num = xzalloc(size * sizeof *num);
    memcpy(ret->num, num, size * sizeof *num);
    ret->size = size;
    ret->sign = sign;

    return ret;
}

struct bksmt_bigint *
bksmt_bigint_add(struct bksmt_bigint *i1, struct bksmt_bigint *i2)
{
    struct bksmt_bigint *i3;
    
    /* obtain a zero bigint */
    i3 = bksmt_bigint_init_int(0);

    /* add i1 and i2 to i3 */
    bksmt_bigint_adds(i3, i1);
    bksmt_bigint_adds(i3, i2);
    return i3;
}
struct bksmt_bigint *
bksmt_bigint_dup(struct bksmt_bigint *src)
{
    return bksmt_bigint_init_lst(src->num, src->size, src->sign);
}

void
bksmt_bigint_adds(struct bksmt_bigint *dst, struct bksmt_bigint *src)
{
    struct bksmt_bigint *sto;
    if (dst->sign == src->sign) 
        /* if signs are equal, then this amounts to simple addition of num arrays */
        adds(dst, src);
    else if (geq(dst, src))
        /* if signs are unequal, then we need to subtract the num arrays. Algorithm is only feasible if we know which num is larger */
        subs(dst, src);
    else {
        /* duplicate src, sub dst from src and negate sign( since src - dst = -(dst-src) ),
         * then replace dst with sto's info, free copy 
         */
        sto = bksmt_bigint_dup(src); 
        subs(sto, dst);
        sto->sign = (sto->sign == 1) ? 0 : 1;

        free(dst->num);

        dst->num = sto->num;
        dst->size = sto->size;
        dst->sign = sto->sign;

        free(sto);
    }
}

static void 
expand_num(struct bksmt_bigint *src, size_t nsize)
{
    src->num = xrealloc(src->num, src->size * sizeof(*(src->num)), nsize  * sizeof(*(src->num)));
    src->size = nsize;
}

static void 
shrink_num(struct bksmt_bigint *src)
{
    int i;
    for (i = src->size - 1; i >= 0 && src->num[i] == 0; i--)
        ;

    /* special case, if i == src->size - 1, then no resizing to be done */
    if (i == src->size - 1) 
        return;
    
    src->num = xrealloc(src->num, src->size, i + 1);
    src->size = i + 1;
    return;
}

/* perform in place addition of src's num to dst's num */
static void 
adds(struct bksmt_bigint *dst, struct bksmt_bigint *src) {
    int i;
    struct bksmt_bigint *max, *min;
    uint64_t psto, sto, carry;

    max = dst->size < src->size ? src : dst; 
    min = dst == max ? src : dst; 

    /* if the destination is smaller than the max, expand it */
    if (min == dst)
        expand_num(dst, max->size); 

    carry = 0;
    for (i = 0; i < dst->size; i++) {

        psto = (i >= src->size) ? 0 : src->num[i];

        
        sto = psto + dst->num[i] + carry;
        if (UINT64_MAX - dst->num[i] < psto || UINT64_MAX - carry < psto + dst->num[i])
            carry = 1;
        else
            carry = 0;

        dst->num[i] = sto;
    }
    
    if (carry == 1) {
        expand_num(dst, dst->size + 1); 
        dst->num[dst->size - 1] = 1;
   }

}

static void 
subs(struct bksmt_bigint *dst, struct bksmt_bigint *src)
{
//    int i; 
//    uint64_t carry, sto;
//
//    for (i = 0; i < dst->size; i++) {
//        if (i >= src->size)
//            sto = 0;
//        else 
//            sto = src->num[i];
//        
//        sto = dst->num[i] - sto; 
//
//        if (sto > dst->num[i] || sto < src->num[i])
//    }
}

static int 
geq(struct bksmt_bigint *i1, struct bksmt_bigint *i2)
{
    int i;

    if (i1->size > i2->size) 
        return 1;
    if (i2->size > i1->size) 
        return 0;

    for(i = i1->size - 1; i >= 0; i--) {
        if (i1->num[i] > i2->num[i])
            return 1;
        if (i2->num[i] > i1->num[i])
            return 0;
    }

    return 1; 
}

void
bksmt_bigint_lst_rep(struct bksmt_bigint *src, uint64_t **num, size_t *size, int *sign)
{
    *num = xmdup(src->num, src->size * sizeof **num);
    *size = src->size;
    *sign = src->sign;
}


