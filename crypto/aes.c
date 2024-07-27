#include "aes.h"

#include <stdio.h>
#include <string.h>

/* FIPS-197 Section 5.2 */
#define AESNK_128 4
#define AESNK_192 6
#define AESNK_256 8
#define AESNR(nk) (nk) + 6

/* FIPS-197 Section 5.1.1 */
static unsigned char aes_subbytes_lut[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};

/* FIPS-197 Section 5.3.2 */
static unsigned char aes_inv_subbytes_lut[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
};

/* FIPS-197 Section 5.1.3 */
#define GF_POLY { \
    0x02,         \
    0x01,         \
    0x01,         \
    0x03,         \
}

/* FIPS-197 Section 5.3.3 */
#define GF_INV_POLY { \
    0x0e,             \
    0x09,             \
    0x0d,             \
    0x0b,             \
}

/* FIPS-197 Section 5.2 */
static unsigned char aes_rcon[40] = {
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00,
    0x1b, 0x00, 0x00, 0x00,
    0x36, 0x00, 0x00, 0x00,
};



/* FIPS-197 Section 4.2 */
static unsigned char bin_pmult(unsigned char, unsigned char);

/* FIPS-197 Section 4.3 */
static void byte_pmmult(unsigned char[4], unsigned char[4], unsigned char[4]);

/* 
 * takes an input stream of 16 bytes and converts to a state
 * with the convention s[r,c] = i[r + 4c] 
 */
static void aes_in(unsigned char [16], unsigned char [16]);

/* 
 * takes a state and converts to an output stream of 16
 * bytes with the convention s[r,c] = o[r + 4c] 
 */
static void aes_out(unsigned char [16], unsigned char [16]);

static void aes_kexp(unsigned char *, unsigned char *, int);

static void rotword(unsigned char [4]);

static void subword(unsigned char [4]);

static unsigned char xtimes(unsigned char);

/* each op is in-place */
static void aes_subbytes(unsigned char[16]);

static void aes_shiftrows(unsigned char[16]);

static void aes_mixcols(unsigned char[16]);

static void aes_addroundkey(unsigned char[16], unsigned char[16]);

static void aes_inv_subbytes(unsigned char[16]);

static void aes_inv_shiftrows(unsigned char[16]);

static void aes_inv_mixcols(unsigned char[16]);

static void aes_addroundkey(unsigned char[16], unsigned char[16]);

void 
bksmt_aes_128(unsigned char in[16], unsigned char key[16], unsigned char out[16])
{
    unsigned char state[16], sched[176];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 4);

    /* add first round key */
    aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 10; round++) {
        aes_subbytes(state);
        aes_shiftrows(state);
        aes_mixcols(state);
        aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    aes_subbytes(state);
    aes_shiftrows(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched + 160);

    /* build output stream from state table */
    aes_out(state, out);
}

void 
bksmt_aes_inv_128(unsigned char in[16], unsigned char key[16], unsigned char out[16])
{
    unsigned char state[16], sched[176];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 4);

    /* add first round key */
    aes_addroundkey(state, sched + 160);

    /* do rounds */
    for(round = 9; round > 0; round--) {
        aes_inv_shiftrows(state);
        aes_inv_subbytes(state);
        aes_addroundkey(state, sched + round * 16);
        aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    aes_inv_shiftrows(state);
    aes_inv_subbytes(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched);

    /* build output stream from state table */
    aes_out(state, out);
}

void 
bksmt_aes_192(unsigned char in[16], unsigned char key[24], unsigned char out[16])
{
    unsigned char state[16], sched[208];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 6);

    /* add first round key */
    aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 12; round++) {
        aes_subbytes(state);
        aes_shiftrows(state);
        aes_mixcols(state);
        aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    aes_subbytes(state);
    aes_shiftrows(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched + 192);

    /* build output stream from state table */
    aes_out(state, out);
}

void 
bksmt_aes_inv_192(unsigned char in[16], unsigned char key[24], unsigned char out[16])
{
    unsigned char state[16], sched[208];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 6);

    /* add first round key */
    aes_addroundkey(state, sched + 192);

    /* do rounds */
    for(round = 11; round > 0; round--) {
        aes_inv_shiftrows(state);
        aes_inv_subbytes(state);
        aes_addroundkey(state, sched + round * 16);
        aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    aes_inv_shiftrows(state);
    aes_inv_subbytes(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched);

    /* build output stream from state table */
    aes_out(state, out);
}

void 
bksmt_aes_256(unsigned char in[16], unsigned char key[32], unsigned char out[16])
{
    unsigned char state[16], sched[240];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 8);

    /* add first round key */
    aes_addroundkey(state, sched);

    /* do rounds */
    for(round = 1; round < 14; round++) {
        aes_subbytes(state);
        aes_shiftrows(state);
        aes_mixcols(state);
        aes_addroundkey(state, sched + round * 16);
    }

    /* final round requires special handling */
    aes_subbytes(state);
    aes_shiftrows(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched + 224);

    /* build output stream from state table */
    aes_out(state, out);
}

void 
bksmt_aes_inv_256(unsigned char in[16], unsigned char key[32], unsigned char out[16])
{
    unsigned char state[16], sched[240];
    int round, i, j;

    /* parse input stream into state table */
    aes_in(in, state);

    /* expand key */
    aes_kexp(key, sched, 8);

    /* add first round key */
    aes_addroundkey(state, sched + 224);

    /* do rounds */
    for(round = 13; round > 0; round--) {
        aes_inv_shiftrows(state);
        aes_inv_subbytes(state);
        aes_addroundkey(state, sched + round * 16);
        aes_inv_mixcols(state);
    }

    /* final round requires special handling */
    aes_inv_shiftrows(state);
    aes_inv_subbytes(state);

    /* add last roundkey word */
    aes_addroundkey(state, sched);

    /* build output stream from state table */
    aes_out(state, out);
}



static unsigned char 
bin_pmult(unsigned char p1, unsigned char p2)
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
byte_pmmult(unsigned char p1[4], unsigned char p2[4], unsigned char pret[4]) 
{
    int i;

    /* zero out pret */

    bzero(pret, 4);
    /* FIPS-197 Section 4.3 Fig 4.12 */
    for (i = 0; i < 4; i++) {
       pret[i] ^= bin_pmult(p1[i % 4], p2[0]);
       pret[i] ^= bin_pmult(p1[(i+3) % 4], p2[1]); 
       pret[i] ^= bin_pmult(p1[(i+2) % 4], p2[2]);
       pret[i] ^= bin_pmult(p1[(i+1) % 4], p2[3]); 
    }
}


static void
aes_in(unsigned char in[16], unsigned char state[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            state[4*i + j] = in[4*j + i];
}

static void
aes_out(unsigned char state[16], unsigned char out[16])
{
    int i, j;

    /* i is row #, j is col # */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
           out[i + 4 *j] = state[4*i + j]; 
}


/* nk = number of key words */
static void
aes_kexp(unsigned char *key, unsigned char *sched, int nk)
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
aes_subbytes(unsigned char state[16])
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
aes_shiftrows(unsigned char state[16])
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
aes_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}

static void 
aes_inv_subbytes(unsigned char state[16])
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
aes_inv_shiftrows(unsigned char state[16])
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
aes_inv_mixcols(unsigned char state[16])
{
    unsigned char pconst[4] = GF_INV_POLY;
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++) {
        /* obtain the jth column */
        for (i = 0; i < 4; i++) 
            col[i] = state[4 * i + j];

        /* poly mult jth column by pconst and store in trans */
        byte_pmmult(col, pconst, trans);

        /* return transformed column to state */
        for (i = 0; i < 4; i++) 
            state[4*i + j] = trans[i];
    }

}


static void 
aes_addroundkey(unsigned char state[16], unsigned char roundkey[16])
{
    unsigned char col[4], trans[4];
    int i, j;

    for (j = 0; j < 4; j++)
        for (i = 0; i < 4; i++) 
            state[4 * j + i] ^= roundkey[4*i + j];
}

