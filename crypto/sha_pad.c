#include "sha_pad.h"

#include <stdio.h>

#include "../lib/math.h"
#include "../lib/xmalloc.h"


#include <string.h>

unsigned char * 
bksmt_sha256_pad(unsigned char *msg, long len)
{
    long blen;
    int pblen, applen, i, mask;
    unsigned char *ret;

    /* length in bits */
    blen = len * 8;

    /* padding length in bits */
    pblen = pmod((448 - blen - 1), 512);

    /* appendix len in bytes */
    applen = (pblen + 1 + 64)/8;

    /* make a return msg of size len + appendix len */
    ret = xzallocarray(len + applen, sizeof *ret);

    /* make a return msg of size len + appendix len */
    memcpy(ret, msg, len);

    /* appended 1 */
    ret[len] = 0x80; 

    /* write binary rep of len to end */
    for (i = 0; i < 8; i++) {
        ret[len + applen - 1 - i] = blen >> (i * 8); 
    }

    return ret;
}

long 
bksmt_sha256_pad_len(long len)
{
    long blen, pblen, applen;

    /* orig len in bits */
    blen  = 8 * len;

    /* padding length in bits */
    pblen = pmod((448 - blen - 1), 512);

    /* appendix len in bytes */
    applen = (pblen + 1 + 64)/8;

    return len + applen;
}

unsigned char * 
bksmt_sha512_pad(unsigned char *msg, long len) 
{
    long blen;
    int pblen, applen, i, mask;
    unsigned char *ret;

    /* length in bits */
    blen = len * 8;

    /* padding length in bits */
    pblen = pmod((896 - blen - 1), 1024);

    /* appendix len in bytes */
    applen = (pblen + 1 + 128)/8;

    /* make a return msg of size len + appendix len */
    ret = xzallocarray(len + applen, sizeof *ret);

    /* make a return msg of size len + appendix len */
    memcpy(ret, msg, len);

    /* appended 1 */
    ret[len] = 0x80; 

    /* write binary rep of len to end */
    for (i = 0; i < 8; i++) {
        ret[len + applen - 1 - i] = blen >> (i * 8); 
    }

    return ret;
}

long 
bksmt_sha512_pad_len(long len)
{
    long blen, pblen, applen;

    /* orig len in bits */
    blen  = 8 * len;

    /* padding length in bits */
    pblen = pmod((896 - blen - 1), 1024);

    /* appendix len in bytes */
    applen = (pblen + 1 + 128)/8;

    return len + applen;
}
