#include "../../crypto/sha.h"

#include <string.h>
#include <stdio.h>

int
main(void)
{
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[48];
    unsigned char res1ch[48] = { 0xca, 0x73, 0x7f, 0x10, 0x14, 0xa4, 0x8f, 0x4c, 0x0b, 0x6d, 0xd4, 0x3c, 0xb1, 0x77, 0xb0, 0xaf, 0xd9, 0xe5, 0x16, 0x93, 0x67, 0x54, 0x4c, 0x49, 0x40, 0x11, 0xe3, 0x31, 0x7d, 0xbf, 0x9a, 0x50, 0x9c, 0xb1, 0xe5, 0xdc, 0x1e, 0x85, 0xa9, 0x41, 0xbb, 0xee, 0x3d, 0x7f, 0x2a, 0xfb, 0xc9, 0xb1 };

    bksmt_sha384_multi(test1, 5, test1 + 5, 20, test1 + 25, 43 - 25, res1);

    if (memcmp(res1, res1ch, 48) != 0)
        return 1;

    bksmt_sha384_multi(test1, 1, test1 + 1, 1, test1 + 2, 43 - 2, res1);

    if (memcmp(res1, res1ch, 48) != 0)
        return 2;


    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[48];
    unsigned char res2ch[48] = { 0x4c, 0x18, 0x42, 0x12, 0x06, 0x17, 0xcb, 0x8e, 0xac, 0xf9, 0x36, 0x0b, 0x48, 0xb9, 0xd7, 0x0a, 0x51, 0x4a, 0x1c, 0x65, 0xdf, 0x38, 0xe8, 0xa9, 0xea, 0x8c, 0x15, 0x5d, 0xe4, 0x23, 0xb9, 0xee, 0x55, 0x53, 0x98, 0xd5, 0x01, 0xf1, 0x57, 0x94, 0x85, 0xa9, 0x23, 0x22, 0xe8, 0xa9, 0x9f, 0xa9 };

    bksmt_sha384_multi(test2, 158, test2 + 158, 10, test2 + 168, 10, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 3;

    bksmt_sha384_multi(test2, 64, test2 + 64, 64, test2 + 128, 50, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 4;

    bksmt_sha384_multi(test2, 50, test2 + 50, 1, test2 + 51, 178-51, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 5;

    bksmt_sha384_multi(test2, 64, test2 + 64, 1, test2 + 65, 178-65, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 5;

    bksmt_sha384_multi(test2, 0, test2 + 0, 64, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 6;
 
    bksmt_sha384_multi(test2, 64, test2 + 64, 0, test2 + 64, 178-64, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 7;
 
    bksmt_sha384_multi(test2, 64, test2 + 64, 178-64, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 8;

    bksmt_sha384_multi(test2, 178, test2 + 178, 0, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 9;
 
    bksmt_sha384_multi(test2, 0, test2, 178, test2 + 178, 0, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 10;
 
    bksmt_sha384_multi(test2, 0, test2, 0, test2, 178, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 11;

    bksmt_sha384_multi(test2, 1, test2 + 1, 88, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 12;

    bksmt_sha384_multi(test2, 88, test2 + 88, 1, test2 + 89, 89, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 13;

    bksmt_sha384_multi(test2, 88, test2 + 88, 89, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 14;

    bksmt_sha384_multi(test2, 176, test2 + 176, 1, test2 + 177, 1, res2);

    if (memcmp(res2, res2ch, 48) != 0)
        return 15;

    char test3[128] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";
    unsigned char res3[48];
    unsigned char res3ch[48] = {  0x70, 0x24, 0x1c, 0xf0, 0x90, 0x70, 0x18, 0xd1, 0x8c, 0xc4, 0xb0, 0x63, 0x6a, 0x15, 0x48, 0x77, 0x0e, 0x44, 0xad, 0x76, 0xf1, 0x66, 0x79, 0x5c, 0x02, 0xa1, 0xb8, 0xb8, 0xe0, 0x48, 0x37, 0x62, 0x13, 0x4b, 0x9b, 0x14, 0xe3, 0x67, 0x2f, 0x5c, 0x63, 0x6e, 0x04, 0x8d, 0x75, 0x98, 0xb4, 0x07 };

    bksmt_sha384_multi(test3, 128, test3 + 128, 0, test3 + 128, 0, res3);

    if (memcmp(res3, res3ch, 48) != 0)
        return 16;

    char test4[120] = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    unsigned char res4[48];
    unsigned char res4ch[48] = { 0x38, 0xb2, 0xee, 0x1d, 0x37, 0x4c, 0xd3, 0xfc, 0xdc, 0xaa, 0xaa, 0xb6, 0x92, 0x81, 0xb2, 0xe2, 0x48, 0x6e, 0x7e, 0xb8, 0x86, 0x80, 0x74, 0x99, 0xf2, 0xa4, 0x12, 0x7e, 0xb8, 0x96, 0x0f, 0x08, 0x90, 0xfb, 0x07, 0xdf, 0x5d, 0x87, 0x5d, 0xda, 0x12, 0x9c, 0x7f, 0x70, 0x7d, 0x06, 0x87, 0x31 };

    bksmt_sha384_multi(test4, 65, test4 + 65, 30, test4 + 95, 25, res4);

    if (memcmp(res4, res4ch, 48) != 0)
        return 17;

    return 0;
}
