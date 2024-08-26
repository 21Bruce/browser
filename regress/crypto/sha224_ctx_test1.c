#include "../../crypto/sha.h"

#include <string.h>

int
main(void)
{
    struct bksmt_sha256_ctx ctx;
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[28];
    unsigned char res1ch[28] = { 0x73, 0x0e, 0x10, 0x9b, 0xd7, 0xa8, 0xa3, 0x2b, 0x1c, 0xb9, 0xd9, 0xa0, 0x9a, 0xa2, 0x32, 0x5d, 0x24, 0x30, 0x58, 0x7d, 0xdb, 0xc0, 0xc3, 0x8b, 0xad, 0x91, 0x15, 0x25 };

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test1, 43);
    bksmt_sha224_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 1;

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test1, 20);
    bksmt_sha224_ctx_hash(&ctx, test1 + 20, 23);
    bksmt_sha224_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 28) != 0)
        return 2;

    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[28];
    unsigned char res2ch[28] = { 0x88, 0x3c, 0x6b, 0xec, 0x0b, 0x28, 0x0d, 0xf5, 0x51, 0x46, 0x15, 0xae, 0xb9, 0x3b, 0xaf, 0x52, 0x2e, 0x67, 0x17, 0x35, 0xae, 0x98, 0x63, 0x09, 0x2d, 0x4d, 0xf1, 0x17 };

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test2, 178);
    bksmt_sha224_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 3;

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test2, 64);
    bksmt_sha224_ctx_hash(&ctx, test2 + 64, 63);
    bksmt_sha224_ctx_hash(&ctx, test2 + 127, 1);
    bksmt_sha224_ctx_hash(&ctx, test2 + 128, 50);
    bksmt_sha224_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 28) != 0)
        return 4;

    char test3[63] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    unsigned char res3[28];
    unsigned char res3ch[28] = { 0x1d, 0x4e, 0x05, 0x1f, 0x4d, 0x6f, 0xed, 0x2a, 0x63, 0xfd, 0x24, 0x21, 0xe6, 0x58, 0x34, 0xce, 0xc0, 0x0d, 0x64, 0x45, 0x65, 0x53, 0xde, 0x34, 0x96, 0xae, 0x8b, 0x1d };

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test3, 63);
    bksmt_sha224_ctx_finish(&ctx, res3);

    if (memcmp(res3, res3ch, 28) != 0)
        return 4;

    bksmt_sha224_ctx_init(&ctx);
    bksmt_sha224_ctx_hash(&ctx, test3, 1);
    bksmt_sha224_ctx_hash(&ctx, test3, 61);
    bksmt_sha224_ctx_hash(&ctx, test3, 1);
    bksmt_sha224_ctx_finish(&ctx, res3);
 
    if (memcmp(res3, res3ch, 28) != 0)
        return 5;
 
    return 0;
}
