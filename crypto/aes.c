#include "aes.h"

#include <stdio.h>
#include <string.h>


/* FIPS-197 Section 4.2 */
static unsigned char bksmt_bin_pmult(unsigned char, unsigned char);

/* FIPS-197 Section 4.3 */
static void bksmt_byte_pmmult(unsigned char[4], unsigned char[4], unsigned char[4]);

/* 
 * takes an input stream of 16 bytes and converts to a state
 * with the convention s[r,c] = i[r + 4c] 
 */
static void bksmt_aes_in(unsigned char [16], unsigned char [16]);

/* 
 * takes a state and converts to an output stream of 16
 * bytes with the convention s[r,c] = o[r + 4c] 
 */
static void bksmt_aes_out(unsigned char [16], unsigned char [16]);

static void bksmt_aes_kexp(unsigned char *, unsigned char *, int);

static void rotword(unsigned char [4]);

static void subword(unsigned char [4]);

static unsigned char xtimes(unsigned char);

/* each op is in-place */
static void bksmt_aes_subbytes(unsigned char[16]);

static void bksmt_aes_shiftrows(unsigned char[16]);

static void bksmt_aes_mixcols(unsigned char[16]);

static void bksmt_aes_addroundkey(unsigned char[16], unsigned char[16]);

static void bksmt_aes_inv_subbytes(unsigned char[16]);

static void bksmt_aes_inv_shiftrows(unsigned char[16]);

static void bksmt_aes_inv_mixcols(unsigned char[16]);

static void bksmt_aes_addroundkey(unsigned char[16], unsigned char[16]);




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
bksmt_aes_inv_128(unsigned char in[16], unsigned char key[16], unsigned char out[16])
{
    unsigned char state[16], sched[176];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 4);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched + 160);

    /* do rounds */
    for(round = 9; round > 0; round--) {
        bksmt_aes_inv_shiftrows(state);
        bksmt_aes_inv_subbytes(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
        bksmt_aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    bksmt_aes_inv_shiftrows(state);
    bksmt_aes_inv_subbytes(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched);

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
bksmt_aes_inv_192(unsigned char in[16], unsigned char key[24], unsigned char out[16])
{
    unsigned char state[16], sched[208];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 6);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched + 192);

    /* do rounds */
    for(round = 11; round > 0; round--) {
        bksmt_aes_inv_shiftrows(state);
        bksmt_aes_inv_subbytes(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
        bksmt_aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    bksmt_aes_inv_shiftrows(state);
    bksmt_aes_inv_subbytes(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched);

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

void 
bksmt_aes_inv_256(unsigned char in[16], unsigned char key[32], unsigned char out[16])
{
    unsigned char state[16], sched[240];
    int round, i, j;

    /* parse input stream into state table */
    bksmt_aes_in(in, state);

    /* expand key */
    bksmt_aes_kexp(key, sched, 8);

    /* add first round key */
    bksmt_aes_addroundkey(state, sched + 224);

    /* do rounds */
    for(round = 13; round > 0; round--) {
        bksmt_aes_inv_shiftrows(state);
        bksmt_aes_inv_subbytes(state);
        bksmt_aes_addroundkey(state, sched + round * 16);
        bksmt_aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    bksmt_aes_inv_shiftrows(state);
    bksmt_aes_inv_subbytes(state);

    /* add last roundkey word */
    bksmt_aes_addroundkey(state, sched);

    /* build output stream from state table */
    bksmt_aes_out(state, out);
}



static unsigned char 
bksmt_bin_pmult(unsigned char p1, unsigned char p2)
{
    unsigned char basem[8];
    unsigned char ret;
    int i, coeff;

    /* compute p1 * x^n for n in {1, ..., x**7} */
    basem[0] = p1; 
    for(i = 1; i < 8; i++)
        basem[i] = xtimes(basem[i-1]);

    /* use the above, decompose p2 into a_0 ^ a_1x ^ ... a_7x**7, and use p1 * p2 = a_0p1 ^ a_2x*p1 ^ ... */
    ret = 0;
    for(i = 0; i < 8; i++) {
        coeff = (p2 & (1 << i)) >> i;
        ret ^= coeff * basem[i];
    }

    return ret;
}

static unsigned char 
xtimes(unsigned char p1)
{
    int lst;

    /* bit at idx 7, i.e. x**7 coeff */
    lst = p1 >> 7;

    /* multiply by x */
    p1 = p1 << 1;

    /* if lst = 1, then we need to reduce by the poly 0x1b, else we good */
    p1 = lst ? p1 ^ 0x1b : p1; 

    return p1;
}

static void
bksmt_byte_pmmult(unsigned char p1[4], unsigned char p2[4], unsigned char pret[4]) 
{
    int i;

    /* zero out pret */

    bzero(pret, 4);
    /* FIPS-197 Section 4.3 Fig 4.12 */
    for (i = 0; i < 4; i++) {
       pret[i] ^= bksmt_bin_pmult(p1[i % 4], p2[0]);
       pret[i] ^= bksmt_bin_pmult(p1[(i+3) % 4], p2[1]); 
       pret[i] ^= bksmt_bin_pmult(p1[(i+2) % 4], p2[2]);
       pret[i] ^= bksmt_bin_pmult(p1[(i+1) % 4], p2[3]); 
    }
}


static void
bksmt_aes_in(unsigned char in[16], unsigned char state[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            state[4*i + j] = in[4*j + i];
}

static void
bksmt_aes_out(unsigned char state[16], unsigned char out[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
           out[i + 4 *j] = state[4*i + j]; 
}


/* nk = number of key words */
static void
bksmt_aes_kexp(unsigned char *key, unsigned char *sched, int nk)
{
    unsigned char temp[4];
    int nr, i, j, ksize, ssize;


    /* we can derive all byte sizes of fields from nk + nr */
    nr = AESNR(nk);
    ksize = 4 * nk;
    ssize = 16 * (nr + 1);

    for(i = 0; i < nk; i++)
        /* cp key word into sched word */
        for (j = 0; j < 4; j++)
            sched[4*i + j] = key[i * 4 + j];
    
    for(i = nk; i < 4 * (nr+1); i++) {
        /* cp key word into sched word */
        for (j = 0; j < 4; j++)
            temp[j] = sched[(i-1) * 4 + j];
       if (i % nk == 0) {
            rotword(temp);
            subword(temp);
            for (j = 0; j < 4; j++)
                temp[j] ^= aes_rcon[4 * ((i/nk) - 1) + j];
        }
        else if (nk > 6 && i % nk == 4) {
            subword(temp);
        }
        for (j = 0; j < 4; j++)
           sched[4*i + j] = sched[(i-nk) * 4 + j] ^ temp[j];
    }
}

static void 
rotword(unsigned char word[4]) 
{
    unsigned char tmp;

    tmp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;
}

static void 
subword(unsigned char word[4])
{
    int i, r, c;

    for (i = 0; i < 4; i++) {
        r = (word[i] & 0xF0) >> 4;
        c = word[i] & 0x0F;
        word[i] = aes_subbytes_lut[16*r + c];
    }
}

static void
bksmt_aes_subbytes(unsigned char state[16])
{
    int r, c, i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++) {
            r = (state[4*i + j] & 0xF0) >> 4;
            c = state[4*i + j] & 0x0F;
            state[4*i + j] = aes_subbytes_lut[16*r + c];
        }
}

static void
bksmt_aes_shiftrows(unsigned char state[16])
{
    unsigned char tmp;

    /* row 1, rotate one left */
    tmp = state[4];
    state[4] = state[5];
    state[5] = state[6];
    state[6] = state[7];
    state[7] = tmp;

    /* 
     * row 2, rotate two left, equivalent of swapping
     * 0,2 and 1,3 -> 8,10 and 9,11 in idx form
     */
    tmp = state[8];
    state[8] = state[10];
    state[10] = tmp;
    tmp = state[9];
    state[9] = state[11];
    state[11] = tmp;

    /* 
     * row 3, rotate three left, equivalent of 
     * rotate 1 right.
     */
    tmp = state[12];
    state[12] = state[15];
    state[15] = state[14];
    state[14] = state[13];
    state[13] = tmp;
}

static void
bksmt_aes_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        bksmt_byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}

static void 
bksmt_aes_inv_subbytes(unsigned char state[16])
{
    int r, c, i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++) {
            r = (state[4*i + j] & 0xF0) >> 4;
            c = state[4*i + j] & 0x0F;
            state[4*i + j] = aes_inv_subbytes_lut[16*r + c];
        }
}

static void 
bksmt_aes_inv_shiftrows(unsigned char state[16])
{
    unsigned char temp;

    /* first row, one shift right */
    temp = state[4];
    state[4] = state[7];
    state[7] = state[6];
    state[6] = state[5];
    state[5] = temp;

    /* first row, two shift right */
    temp = state[8];
    state[8] = state[10];
    state[10] = temp;
    temp = state[9];
    state[9] = state[11];
    state[11] = temp;

    /* first row, three shift right, equivalent to one shift left */
    temp = state[12];
    state[12] = state[13];
    state[13] = state[14];
    state[14] = state[15];
    state[15] = temp;
}

static void
bksmt_aes_inv_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_INV_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        bksmt_byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}


static void 
bksmt_aes_addroundkey(unsigned char state[16], unsigned char roundkey[16])
{
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++)
        for (i = 0; i < 4; i++) 
            state[4 * j + i] ^= roundkey[4*i + j];
}

