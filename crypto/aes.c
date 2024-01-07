#include "aes.h"

#include "aes_kexp.h"
#include "aes_const.h"
#include "aes_io.h"
#include "aes_op.h"
#include <stdio.h>

void 
bksmt_aes_128(unsigned char in[16], unsigned char key[16], unsigned char out[16])
{
    unsigned char state[16], sched[176];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 4);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 10; round++) {
        bksmt_aes_subbytes(state);
        bksmt_aes_shiftrows(state);
        bksmt_aes_mixcols(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    bksmt_aes_subbytes(state);
    bksmt_aes_shiftrows(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched + 160);

    /* build output stream from state table */
    bksmt_aes_out(state, out);
}

void 
bksmt_aes_192(unsigned char in[16], unsigned char key[24], unsigned char out[16])
{
    unsigned char state[16], sched[208];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 6);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 12; round++) {
        bksmt_aes_subbytes(state);
        bksmt_aes_shiftrows(state);
        bksmt_aes_mixcols(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    bksmt_aes_subbytes(state);
    bksmt_aes_shiftrows(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched + 192);

    /* build output stream from state table */
    bksmt_aes_out(state, out);
}

void 
bksmt_aes_256(unsigned char in[16], unsigned char key[32], unsigned char out[16])
{
    unsigned char state[16], sched[240];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 8);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 14; round++) {
        bksmt_aes_subbytes(state);
        bksmt_aes_shiftrows(state);
        bksmt_aes_mixcols(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    bksmt_aes_subbytes(state);
    bksmt_aes_shiftrows(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched + 224);

    /* build output stream from state table */
    bksmt_aes_out(state, out);
}
