#include "../../crypto/sha.h"

#include <string.h>
#include <stdio.h>

int
main(void)
{
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[28];
    unsigned char res1ch[28] = { 0x94, 0x4c, 0xd2, 0x84, 0x7f, 0xb5, 0x45, 0x58, 0xd4, 0x77, 0x5d, 0xb0, 0x48, 0x5a, 0x50, 0x00, 0x31, 0x11, 0xc8, 0xe5, 0xda, 0xa6, 0x3f, 0xe7, 0x22, 0xc6, 0xaa, 0x37 };

    bksmt_sha512t224_multi(test1, 5, test1 + 5, 20, test1 + 25, 43 - 25, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 1;

    bksmt_sha512t224_multi(test1, 1, test1 + 1, 1, test1 + 2, 43 - 2, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 2;


    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[28];
    unsigned char res2ch[28] = { 0x31, 0xe9, 0x68, 0xa2, 0x16, 0x16, 0xb5, 0x9a, 0xc3, 0xbc, 0xba, 0xf4, 0xff, 0x2f, 0x3e, 0x09, 0x8b, 0x35, 0x62, 0xe9, 0x8d, 0x62, 0x89, 0x6f, 0x0f, 0x58, 0xe8, 0x29 };

    bksmt_sha512t224_multi(test2, 158, test2 + 158, 10, test2 + 168, 10, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 3;

    bksmt_sha512t224_multi(test2, 64, test2 + 64, 64, test2 + 128, 50, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 4;

    bksmt_sha512t224_multi(test2, 50, test2 + 50, 1, test2 + 51, 178-51, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 5;

    bksmt_sha512t224_multi(test2, 64, test2 + 64, 1, test2 + 65, 178-65, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 5;

    bksmt_sha512t224_multi(test2, 0, test2 + 0, 64, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 6;
 
    bksmt_sha512t224_multi(test2, 64, test2 + 64, 0, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 7;
 
    bksmt_sha512t224_multi(test2, 64, test2 + 64, 178-64, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 8;

    bksmt_sha512t224_multi(test2, 178, test2 + 178, 0, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 9;
 
    bksmt_sha512t224_multi(test2, 0, test2, 178, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 10;
 
    bksmt_sha512t224_multi(test2, 0, test2, 0, test2, 178, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 11;

    bksmt_sha512t224_multi(test2, 1, test2 + 1, 88, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 12;

    bksmt_sha512t224_multi(test2, 88, test2 + 88, 1, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 13;

    bksmt_sha512t224_multi(test2, 88, test2 + 88, 89, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 14;

    bksmt_sha512t224_multi(test2, 176, test2 + 176, 1, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 15;

    char test3[128] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";
    unsigned char res3[28];
    unsigned char res3ch[28] = { 0xfe, 0xa8, 0x65, 0x59, 0x24, 0x81, 0x7d, 0x1e, 0x33, 0x85, 0x0f, 0xee, 0x56, 0xe4, 0x77, 0xe9, 0x2d, 0x42, 0xe9, 0x02, 0x12, 0x06, 0xe5, 0xc3, 0x0e, 0xb7, 0xa9, 0xf7 };

    bksmt_sha512t224_multi(test3, 128, test3 + 128, 0, test3 + 128, 0, res3);

    if (memcmp(res3, res3ch, 28) != 0)
        return 16;

    char test4[120] = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    unsigned char res4[28];
    unsigned char res4ch[28] = { 0x26, 0x80, 0x1f, 0x25, 0xfe, 0x72, 0xd4, 0xc7, 0x73, 0xc1, 0x42, 0x1a, 0xcf, 0xbc, 0xe0, 0xc7, 0x2a, 0x2a, 0x97, 0xde, 0x85, 0xb4, 0x1c, 0x39, 0x5d, 0xbf, 0x87, 0x59 };

    bksmt_sha512t224_multi(test4, 65, test4 + 65, 30, test4 + 95, 25, res4);

    if (memcmp(res4, res4ch, 28) != 0)
        return 17;

    return 0;
}
