#include "aes_kexp.h"
#include "aes_const.h"

#include <stdio.h>

static void rotword(unsigned char [4]);
static void subword(unsigned char [4]);

/* nk = number of key words */
void
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
