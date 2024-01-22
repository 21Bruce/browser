#ifndef __BKSMT_LIB_LARGE_H__
#define __BKSMT_LIB_LARGE_H__

#include <stdint.h>

/* unbounded int arithmetic functions */

/* 
 * name conventions: 
 *
 *  eq - stores result in first arg
 */

struct bksmt_large;

struct bksmt_large *bksmt_large_init(unsigned char *, int);

void bksmt_large_free(struct bksmt_large *);

struct bksmt_large *bksmt_large_add(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_addeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_neg(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_negeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_sub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_subeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_sub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_subeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_mult(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_multeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_div(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_diveq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_shift(struct bksmt_large *, int);

void bksmt_large_shifteq(struct bksmt_large *, int);

int bksmt_large_eq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_neq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_leq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_lt(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_geq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_gt(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_mod(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_modeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_or(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_oreq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_and(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_andeq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_xor(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_xoreq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_not(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_noteq(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large {
    unsigned char *num;
    int len;
};

#endif /* __BKSMT_LIB_LARGE_H__ */
