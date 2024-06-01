#include "large.h"
#include "math.h"
#include "xmalloc.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void expand_num(struct bksmt_bigint *, size_t);
static void adds(struct bksmt_bigint *, struct bksmt_bigint *);
static void subs(struct bksmt_bigint *, struct bksmt_bigint *);

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

void
bksmt_bigint_adds(struct bksmt_bigint *dst, struct bksmt_bigint *src)
{
    if (dst->sign == src->sign) 
        adds(dst, src);
    else 
        subs(dst, src);
}

static void 
expand_num(struct bksmt_bigint *src, size_t nsize)
{
    src->num = xrealloc(src->num, src->size * sizeof(*(src->num)), nsize  * sizeof(*(src->num)));
    src->size = nsize;
}

static void 
adds(struct bksmt_bigint *dst, struct bksmt_bigint *src) {
    int i;
    struct bksmt_bigint *max, *min;
    uint64_t sto, carry;

    max = dst->size < src->size ? src : dst; 
    min = dst == max ? src : dst; 

    /* if the destination is smaller than the max, expand it */
    if (min == dst)
        expand_num(dst, max->size); 


    carry = 0;
    for (i = 0; i < dst->size; i++) {
        if (i >= src->size)
            sto = 0;
        else 
            sto = src->num[i];
        
        sto += dst->num[i] + carry;
        if (sto < dst->num[i])
            carry = 1;
        else
            carry = 0;
        dst->num[i] = sto;
    }

    if (carry == 1) {
        dst->num = xrealloc(dst->num, dst->size, dst->size + 1);
        dst->num[dst->size] = 1;
        dst->size++;
   }
}

static void 
subs(struct bksmt_bigint *dst, struct bksmt_bigint *src)
{
    return;
}

void
bksmt_bigint_lst_rep(struct bksmt_bigint *src, uint64_t **num, size_t *size, int *sign)
{
    *num = xmdup(src->num, src->size * sizeof **num);
    *size = src->size;
    *sign = src->sign;
}


