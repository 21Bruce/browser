#include "../../crypto/sha.h"

#include <string.h>

int
main(void)
{
    struct bksmt_sha512_ctx ctx;
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[28];
    unsigned char res1ch[28] = { 0x94, 0x4c, 0xd2, 0x84, 0x7f, 0xb5, 0x45, 0x58, 0xd4, 0x77, 0x5d, 0xb0, 0x48, 0x5a, 0x50, 0x00, 0x31, 0x11, 0xc8, 0xe5, 0xda, 0xa6, 0x3f, 0xe7, 0x22, 0xc6, 0xaa, 0x37 };

    bksmt_sha512t224_ctx_init(&ctx);
    bksmt_sha512t224_ctx_hash(&ctx, test1, 43);
    bksmt_sha512t224_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 1;

    bksmt_sha512t224_ctx_init(&ctx);
    bksmt_sha512t224_ctx_hash(&ctx, test1, 20);
    bksmt_sha512t224_ctx_hash(&ctx, test1 + 20, 23);
    bksmt_sha512t224_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 2;

    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[28];
    unsigned char res2ch[28] = { 0x31, 0xe9, 0x68, 0xa2, 0x16, 0x16, 0xb5, 0x9a, 0xc3, 0xbc, 0xba, 0xf4, 0xff, 0x2f, 0x3e, 0x09, 0x8b, 0x35, 0x62, 0xe9, 0x8d, 0x62, 0x89, 0x6f, 0x0f, 0x58, 0xe8, 0x29 };

    bksmt_sha512t224_ctx_init(&ctx);
    bksmt_sha512t224_ctx_hash(&ctx, test2, 178);
    bksmt_sha512t224_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 3;

    bksmt_sha512t224_ctx_init(&ctx);
    bksmt_sha512t224_ctx_hash(&ctx, test2, 128);
    bksmt_sha512t224_ctx_hash(&ctx, test2 + 128, 1);
    bksmt_sha512t224_ctx_hash(&ctx, test2 + 129, 1);
    bksmt_sha512t224_ctx_hash(&ctx, test2 + 130, 48);
    bksmt_sha512t224_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 4;

    char test3[128] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";
    unsigned char res3[28];
    unsigned char res3ch[28] = { 0xfe, 0xa8, 0x65, 0x59, 0x24, 0x81, 0x7d, 0x1e, 0x33, 0x85, 0x0f, 0xee, 0x56, 0xe4, 0x77, 0xe9, 0x2d, 0x42, 0xe9, 0x02, 0x12, 0x06, 0xe5, 0xc3, 0x0e, 0xb7, 0xa9, 0xf7 };

    bksmt_sha512t224_ctx_init(&ctx);
    bksmt_sha512t224_ctx_hash(&ctx, test3, 128);
    bksmt_sha512t224_ctx_finish(&ctx, res3);

    if (memcmp(res3, res3ch, 28) != 0)
        return 5;


    return 0;
}
