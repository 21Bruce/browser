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




    return 0;
}
