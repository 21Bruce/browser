#include "../../crypto/sha.h"

#include <string.h>

int
main(void)
{
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[20];
    unsigned char res1ch[20] = { 0x2f, 0xd4, 0xe1, 0xc6, 0x7a, 0x2d, 0x28, 0xfc, 0xed, 0x84, 0x9e, 0xe1, 0xbb, 0x76, 0xe7, 0x39, 0x1b, 0x93, 0xeb, 0x12 };

    bksmt_sha1(test1, 43, res1);

    if (memcmp(res1, res1ch, 20) != 0)
        return 1;


    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[20];
    unsigned char res2ch[20] = { 0xdc, 0x16, 0x6d, 0xe7, 0x5e, 0xa9, 0x48, 0x8b, 0x89, 0xba, 0x64, 0xd7, 0x10, 0x6f, 0xb6, 0x2a, 0xf8, 0x68, 0x2d, 0xe9 };

    bksmt_sha1(test2, 178, res2);

    if (memcmp(res2, res2ch, 20) != 0)
        return 2;
                      

    char test3[63] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    unsigned char res3[20];
    unsigned char res3ch[20] = { 0x03, 0xf0, 0x9f, 0x5b, 0x15, 0x8a, 0x7a, 0x8c, 0xda, 0xd9, 0x20, 0xbd, 0xdc, 0x29, 0xb8, 0x1c, 0x18, 0xa5, 0x51, 0xf5 };

    bksmt_sha1(test3, 63, res3);

    if (memcmp(res3, res3ch, 20) != 0)
        return 3;
 
    char test4[64] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    unsigned char res4[20];
    unsigned char res4ch[20] = { 0xc2, 0xdb, 0x33, 0x0f, 0x60, 0x83, 0x85, 0x4c, 0x99, 0xd4, 0xb5, 0xbf, 0xb6, 0xe8, 0xf2, 0x9f, 0x20, 0x1b, 0xe6, 0x99 };

    bksmt_sha1(test4, 56, res4);

    if (memcmp(res4, res4ch, 20) != 0)
        return 4;

    return 0;
}
