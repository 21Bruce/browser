#ifndef __BKSMT_LIB_LARGE_H__
#define __BKSMT_LIB_LARGE_H__

#include <stdint.h>

/* unbounded int arithmetic functions */

/* 
 * name conventions: 
 *
 *  s - signed
 *  u - unsigned
 *  eq - stores result in first arg
 */

struct bksmt_large;

struct bksmt_large *bksmt_large_init(int);

void bksmt_large_free(struct bksmt_large *);

struct bksmt_large *bksmt_large_sadd(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_uadd(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqadd(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqadd(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_ssub(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_usub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqsub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqsub(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_ssub(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_usub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqsub(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqsub(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_smult(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_umult(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqmult(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqmult(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_sdiv(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_udiv(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqdiv(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqdiv(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_sshift(struct bksmt_large *, int);

struct bksmt_large *bksmt_large_ushift(struct bksmt_large *, int);

void bksmt_large_seqshift(struct bksmt_large *, int);

void bksmt_large_ueqshift(struct bksmt_large *, int);

int bksmt_large_eq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_neq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_leq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_lt(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_geq(struct bksmt_large *, struct bksmt_large *);

int bksmt_large_gt(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_smod(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large *bksmt_large_umod(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_seqmod(struct bksmt_large *, struct bksmt_large *);

void bksmt_large_ueqmod(struct bksmt_large *, struct bksmt_large *);

struct bksmt_large {
    uint64_t *num;
    int cap;
};

#endif /* __BKSMT_LIB_LARGE_H__ */
