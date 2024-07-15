#include <stdlib.h>
#include <stdint.h>

#include "xmalloc.h"
#include "int.h"

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

struct bksmt_int *
bksmt_int_dup(struct bksmt_int *i)
{
    struct bksmt_int *ret;

    ret = xmalloc(sizeof(*ret));

    ret->num = xmdup(i->num, i->size * sizeof(*(i->num)));
    ret->size = size;
    ret->sign = sign;

    return ret;
}

int
bksmt_int_cmp(struct bksmt_int *i1, struct bksmt_int *i2)
{
    size_t i;

    /* if i1 is pos and i2 neg, then return 1 */
    if (i1->sign > i2->sign)
        return 1;

    /* if i2 is pos and i1 neg, then return -1 */
    if (i2->sign > i1->sign)
        return -1;

    /* if we reach here, i1->sign == i2->sign */

    /* if i1 is larger than i2, return 1 * sign, since for negative numbers\
     * larger mag means less 
     */
    if (i1->size > i2->size)
        return 1 * i1->sign;

    /* smae logic here as previous case but reversed */
    if (i2->size > i1->size)
        return -1 * i1->sign;

    /* if we reach here, i1->sign == i2->sign && i1->size == i2->size */

    /* loop through and compare individual places.
     * if i1 is bigger, then return 1 multiplied by sign */
     * if i2 is bigger, then return -1 multiplied by sign */
    for(i = 0; i < i1->size; i++)
        if (i1->num[i] > i2->num[i])
            return 1 * i1->sign;
        else if (i2->num[i] > i1->num[i])
            return -1 * i1->sign;

    /* if we reach here, numbers are identical */
    return 0;
}



