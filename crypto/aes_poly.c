#include "aes_poly.h"

static unsigned char xtimes(unsigned char);

unsigned char bksmt_bin_pmult(unsigned char p1, unsigned char p2)
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

void 
bksmt_byte_pmmult(unsigned char p1[4], unsigned char p2[4], unsigned char pret[4]) 
{
    int i;

    /* FIPS-197 Section 4.3 Fig 4.12 */
    for (i = 0; i < 4; i++) {
       pret[i] ^= bksmt_bin_pmult(p1[i % 4], p2[0]);
       pret[i] ^= bksmt_bin_pmult(p1[(i+3) % 4], p2[1]); 
       pret[i] ^= bksmt_bin_pmult(p1[(i+2) % 4], p2[2]);
       pret[i] ^= bksmt_bin_pmult(p1[(i+1) % 4], p2[3]); 
    }
}


