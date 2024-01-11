#include "chacha20_blk.h"
#include "chacha20_blk_io.h"
#include "chacha20_blk_op.h"

#include <stdint.h>
#include <string.h>

static void round(uint32_t [16]);

/* RFC 8439 Sec 2.3.1 */
void 
bksmt_chacha20_blk(unsigned char key[32], unsigned char nonce[12], uint32_t blockcount, unsigned char out[64])
{
    uint32_t state[16], statecp[16];
    int i;

    /* read inputs into state */
    bksmt_chacha20_blk_in(key, nonce, blockcount, state);

    /* we need a copy of state for the end */
    memcpy(statecp, state, 64);

    /* 10 iterations of the round function */
    for(i = 0; i < 10; i++)
        round(state);

    /* add initial state back */
    for(i = 0; i < 16; i++)
        state[i] += statecp[i];

    bksmt_chacha20_blk_out(state, out);
}

/* RFC 8439 Sec 2.3.1 */
static void 
round(uint32_t state[16])
{
    bksmt_chacha20_blk_qround(state + 0, state + 4, state + 8, state + 12);
    bksmt_chacha20_blk_qround(state + 1, state + 5, state + 9, state + 13);
    bksmt_chacha20_blk_qround(state + 2, state + 6, state + 10, state + 14);
    bksmt_chacha20_blk_qround(state + 3, state + 7, state + 11, state + 15);
    bksmt_chacha20_blk_qround(state + 0, state + 5, state + 10, state + 15);
    bksmt_chacha20_blk_qround(state + 1, state + 6, state + 11, state + 12);
    bksmt_chacha20_blk_qround(state + 2, state + 7, state + 8, state + 13);
    bksmt_chacha20_blk_qround(state + 3, state + 4, state + 9, state + 14);
}
