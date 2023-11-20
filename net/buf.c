#include "buf.h"
#include "../lib/xmalloc.h"

#include <assert.h>

struct bksmt_buf *
bksmt_buf_init()
{
    return xmalloc(sizeof (struct bksmt_buf));
}

void
bksmt_buf_free(struct bksmt_buf *buf)
{
    assert(buf != NULL);
    free(buf);
}

struct bksmt_buf_chain *
bksmt_buf_chain_init()
{
    return xzalloc(sizeof (struct bksmt_buf_chain));
} 

void
bksmt_buf_chain_add(struct bksmt_buf_chain *chain, struct bksmt_buf *buf)
{
    struct bksmt_buf_chain *nelem;

    assert(chain != NULL);

    if (chain->buf == NULL) {
        chain->buf = buf;
        return;
    }

    nelem = bksmt_buf_chain_init();
    nelem->buf = buf;
    nelem->nxt = chain->nxt;  
    chain->nxt = nelem;
}

void 
bksmt_buf_chain_release(struct bksmt_buf_chain *chain)
{
    struct bksmt_buf_chain *c, *ctmp;

    assert(chain != NULL);

    for (c = chain; c != NULL; c = ctmp) {
        ctmp = c->nxt;
        free(c);
    }
}
