#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "xmalloc.h"
#include "int.h"

/* expand the int's internal array to size w/leading zeroes */
static void expand_num(struct bksmt_int *, size_t);
/* shrink the int's internal array by removing leading zeroes */
static void shrink_num(struct bksmt_int *);
/* add first's number array to second's number array interpreted as uints */
static void adds_base(struct bksmt_int *, struct bksmt_int *);
/* sub second's number array from first's number array interpreted as uints, w/proper sign flip */
static void subs_base(struct bksmt_int *, struct bksmt_int *);

struct bksmt_int *
bksmt_int_init_int(int64_t num)
{
    struct bksmt_int *ret;

    ret = xmalloc(sizeof *ret);

    ret->num = xmalloc(sizeof *(ret->num));
    ret->size = 1;

    ret->sign = num < 0 ? -1 : 1;
    ret->num[0] = num * ret->sign;

    return ret;
}

struct bksmt_int *
bksmt_int_init_lst(uint64_t *numlist, size_t size, int sign)
{
    struct bksmt_int *ret;

    ret = xmalloc(sizeof *ret);

    ret->num = xmdup(numlist, size * sizeof(*numlist));
    ret->size = size;
    ret->sign = sign;

    return ret;
}

void
bksmt_int_lst_rep(struct bksmt_int *src, uint64_t **numlist, size_t *size, int *sign)
{
    *numlist = xmdup(src->num, src->size * sizeof(*(src->num)));
    *size = src->size;
    *sign = src->sign;
}

struct bksmt_int *
bksmt_int_dup(struct bksmt_int *i)
{
    struct bksmt_int *ret;

    ret = xmalloc(sizeof(*ret));

    ret->num = xmdup(i->num, i->size * sizeof(*(i->num)));
    ret->size = i->size;
    ret->sign = i->sign;

    return ret;
}

int
bksmt_int_cmp(struct bksmt_int *i1, struct bksmt_int *i2)
{
    int i;


    /* if i1 is pos and i2 neg, then return 1 */
    if (i1->sign > i2->sign)
        return 1;


    /* if i2 is pos and i1 neg, then return -1 */
    if (i2->sign > i1->sign)
        return -1;



    /* if we reach here, i1->sign == i2->sign */

    /* if i1 is larger than i2, return 1 * sign, since for negative numbers
     * larger mag means less 
     */
    if (i1->size > i2->size)
        return 1 * i1->sign;


    /* smae logic here as previous case but reversed */
    if (i2->size > i1->size)
        return -1 * i1->sign;

    /* if we reach here, i1->sign == i2->sign && i1->size == i2->size */

    /* loop through and compare individual places.
     * if i1 is bigger, then return 1 multiplied by sign 
     * if i2 is bigger, then return -1 multiplied by sign 
     */
    for(i = (i1->size)-1; i >= 0; i--) {
        if ((i1->num)[i] > (i2->num)[i])
            return 1 * i1->sign;
        else if ((i2->num)[i] > (i1->num)[i])
            return -1 * i1->sign;
    }


    /* if we reach here, numbers are identical */
    return 0;
}

void
bksmt_int_adds(struct bksmt_int *i1, struct bksmt_int *i2)
{
    if (i1->sign == i2->sign) 
        adds_base(i1, i2);
    else if (i2->sign == -1) 
        subs_base(i1, i2);
    else {
        subs_base(i1, i2);
        i1->sign *= -1;
    }
}


static void 
expand_num(struct bksmt_int *i, size_t size)
{
    int j;

    /* realloc to correct size */
    i->num = xrealloc(i->num, sizeof(*(i->num)) * i->size, sizeof(*(i->num)) * size);

    /* zero out new slots */
    for(j = i->size; j < size; j++)
       i->num[j] = 0; 

    /* set new size */
    i->size = size;
}

static void 
shrink_num(struct bksmt_int *i)
{
    int j;
    /* descend internal num array looking for zeros, stopping at first idx, since 0 is a special case */
    for (j = i->size-1; j > 0; j--) {
        if (i->num[j] != 0) break;
    }

    /* if we stopped at the first idx, this is num = {0}, so return */
    /* if we stopped at the last idx, then num is zero truncated, so return */
    if (j == 0 || j == i->size-1) return;

    /* realloc to correct size --- should be j + 1 */
    i->num = xrealloc(i->num, sizeof(*(i->num)) * i->size, sizeof(*(i->num)) * (j+1));
    i->size = j+1;
}

static void 
adds_base(struct bksmt_int *i1, struct bksmt_int *i2)
{
    int i;
    uint64_t carry, storage;

    /* if i1 is < i2 in size, expand it so we can perform addition,
     * if i1 is not smaller, we can j assume i2 is zero for indices above
     * its size
     */
    if (i1->size < i2->size)
        expand_num(i1, i2->size);

    carry = 0;
    for(i = 0; i < i1->size; i++) {
        /* add carry to i2 num, checking if i2 is vitually 0 */
        storage = carry + ((i >= i2->size) ? 0 : i2->num[i]);
        /* check for carry overflow */
        carry = storage < carry ? 1 : 0; 
        /* add i1 num */
        storage += i1->num[i];
        /* check for overflow */
        carry = storage < i1->num[i] ? 1 : carry;
        /* set i1 num to the sum */
        i1->num[i] = storage;
    }

    /* if we have leftover carry, expand the num by 1 and add 1 to the end */
    if (carry == 1) {
        expand_num(i1, i1->size + 1);
        i1->num[i1->size-1] = 1;
    }

}

static void 
subs_base(struct bksmt_int *i1, struct bksmt_int *i2)
{
}

void 
bksmt_int_free(struct bksmt_int *i1)
{
    free(i1->num);
    free(i1);
}
