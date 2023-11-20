#ifndef __BKSMRT_XMALLOC_H__
#define __BKSMRT_XMALLOC_H__

#include <stdlib.h>

void *xmalloc(size_t);
void *xmallocarray(size_t, size_t);
void *xzallocarray(size_t, size_t);

#endif
