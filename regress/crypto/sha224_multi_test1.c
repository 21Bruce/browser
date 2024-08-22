#include "../../crypto/sha.h"

#include <string.h>
#include <stdio.h>

int
main(void)
{
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[28];
    unsigned char res1ch[28] = { 0x73, 0x0e, 0x10, 0x9b, 0xd7, 0xa8, 0xa3, 0x2b, 0x1c, 0xb9, 0xd9, 0xa0, 0x9a, 0xa2, 0x32, 0x5d, 0x24, 0x30, 0x58, 0x7d, 0xdb, 0xc0, 0xc3, 0x8b, 0xad, 0x91, 0x15, 0x25 };

    bksmt_sha224_multi(test1, 5, test1 + 5, 20, test1 + 25, 43 - 25, res1);
    if (memcmp(res1, res1ch, 28) != 0)
        return 1;

    bksmt_sha224_multi(test1, 1, test1 + 1, 1, test1 + 2, 43 - 2, res1);
    if (memcmp(res1, res1ch, 28) != 0)
        return 2;

    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[28];
    unsigned char res2ch[28] = { 0x88, 0x3c, 0x6b, 0xec, 0x0b, 0x28, 0x0d, 0xf5, 0x51, 0x46, 0x15, 0xae, 0xb9, 0x3b, 0xaf, 0x52, 0x2e, 0x67, 0x17, 0x35, 0xae, 0x98, 0x63, 0x09, 0x2d, 0x4d, 0xf1, 0x17 };

    bksmt_sha224_multi(test2, 158, test2 + 158, 10, test2 + 168, 10, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 3;

    bksmt_sha224_multi(test2, 64, test2 + 64, 64, test2 + 128, 50, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 4;

    bksmt_sha224_multi(test2, 50, test2 + 50, 1, test2 + 51, 178-51, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 5;

    bksmt_sha224_multi(test2, 64, test2 + 64, 1, test2 + 65, 178-65, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 5;

    bksmt_sha224_multi(test2, 0, test2 + 0, 64, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 6;
 
    bksmt_sha224_multi(test2, 64, test2 + 64, 0, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 7;
 
    bksmt_sha224_multi(test2, 64, test2 + 64, 178-64, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 8;

    bksmt_sha224_multi(test2, 178, test2 + 178, 0, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 9;
 
    bksmt_sha224_multi(test2, 0, test2, 178, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 10;
 
    bksmt_sha224_multi(test2, 0, test2, 0, test2, 178, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 11;

    bksmt_sha224_multi(test2, 1, test2 + 1, 88, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 12;

    bksmt_sha224_multi(test2, 88, test2 + 88, 1, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 13;

    bksmt_sha224_multi(test2, 88, test2 + 88, 89, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 14;

    bksmt_sha224_multi(test2, 176, test2 + 176, 1, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 15;
 
    char test3[63] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    unsigned char res3[28];
    unsigned char res3ch[28] = { 0x1d, 0x4e, 0x05, 0x1f, 0x4d, 0x6f, 0xed, 0x2a, 0x63, 0xfd, 0x24, 0x21, 0xe6, 0x58, 0x34, 0xce, 0xc0, 0x0d, 0x64, 0x45, 0x65, 0x53, 0xde, 0x34, 0x96, 0xae, 0x8b, 0x1d };

    bksmt_sha224_multi(test3, 1, test3 + 1, 1, test3 + 2, 61, res3);

    if (memcmp(res3, res3ch, 28) != 0)
        return 16;

    bksmt_sha224_multi(test3, 61, test3 + 61, 1, test3 + 62, 1, res3);

    if (memcmp(res3, res3ch, 28) != 0)
        return 17;
 
    return 0;
}
