#include "../../crypto/sha.h"

#include <string.h>

int
main(void)
{
    struct bksmt_sha256_ctx ctx;
    char test1[43] = "The quick brown fox jumps over the lazy dog";     
    unsigned char res1[32];
    unsigned char res1ch[32] = { 0xd7, 0xa8, 0xfb, 0xb3, 0x07, 0xd7, 0x80, 0x94, 0x69, 0xca, 0x9a, 0xbc, 0xb0, 0x08, 0x2e, 0x4f, 0x8d, 0x56, 0x51, 0xe4, 0x6d, 0x3c, 0xdb, 0x76, 0x2d, 0x02, 0xd0, 0xbf, 0x37, 0xc9, 0xe5, 0x92 };

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test1, 43);
    bksmt_sha256_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 32) != 0)
        return 1;

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test1, 20);
    bksmt_sha256_ctx_hash(&ctx, test1 + 20, 23);
    bksmt_sha256_ctx_finish(&ctx, res1);

    if (memcmp(res1, res1ch, 32) != 0)
        return 2;

    char test2[178] = "dkjfsakfjdfsdfkjsdfkdsjfkdsjkfjwdkfwjokfnwefndwonvdojvnnw;rfen2ofndfkndvnwfoiewfnewklkcnxsiovjoweefiewklfnda;sfnw;lgnro3rhgioq3r;gnlkdvoqirvljkbwnofi;jleqwm,bdfn2oe;iljfn;oqiewnf";     
    unsigned char res2[32];
    unsigned char res2ch[32] = {  0x92, 0x76, 0xc7, 0x27, 0x85, 0x5e, 0x7b, 0x24, 0x0f, 0x36, 0x21, 0xf7, 0x7f, 0x72, 0xe8, 0x03, 0x23, 0xab, 0x16, 0x55, 0x1e, 0xbd, 0xa2, 0x5c, 0x12, 0xc1, 0xe4, 0xcb, 0x4b, 0x88, 0x32, 0x24 };

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test2, 178);
    bksmt_sha256_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 32) != 0)
        return 3;

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test2, 64);
    bksmt_sha256_ctx_hash(&ctx, test2 + 64, 63);
    bksmt_sha256_ctx_hash(&ctx, test2 + 127, 1);
    bksmt_sha256_ctx_hash(&ctx, test2 + 128, 50);
    bksmt_sha256_ctx_finish(&ctx, res2);

    if (memcmp(res2, res2ch, 32) != 0)
        return 5;

    char test3[63] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    unsigned char res3[32];
    unsigned char res3ch[32] = { 0x7d, 0x3e, 0x74, 0xa0, 0x5d, 0x7d, 0xb1, 0x5b, 0xce, 0x4a, 0xd9, 0xec, 0x06, 0x58, 0xea, 0x98, 0xe3, 0xf0, 0x6e, 0xee, 0xcf, 0x16, 0xb4, 0xc6, 0xff, 0xf2, 0xda, 0x45, 0x7d, 0xdc, 0x2f, 0x34 };

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test3, 63);
    bksmt_sha256_ctx_finish(&ctx, res3);

    if (memcmp(res3, res3ch, 32) != 0)
        return 6;

    bksmt_sha256_ctx_init(&ctx);
    bksmt_sha256_ctx_hash(&ctx, test3, 1);
    bksmt_sha256_ctx_hash(&ctx, test3, 61);
    bksmt_sha256_ctx_hash(&ctx, test3, 1);
    bksmt_sha256_ctx_finish(&ctx, res3);
 
    if (memcmp(res3, res3ch, 32) != 0)
        return 7;
 
    return 0;
}
