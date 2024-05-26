#include "../../lib/dict.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int
main(void) 
{
    struct bksmt_dict *dhandle = bksmt_dict_init();
    struct bksmt_dict *d2handle = bksmt_dict_init();
    char key1[] = "k1", key2[] = "k2", val1[] = "v1", val2[] = "v2";
    char *box;

    if (bksmt_dict_get(dhandle, key1) != NULL) { 
        fprintf(stderr, "bksmt_dict_get == NULL test FAILED\n");
        return 1;
    }

    bksmt_dict_set(dhandle, key1, val1);

    if (strcmp(bksmt_dict_get(dhandle, key1), val1) != 0) { 
        fprintf(stderr, "bksmt_dict_get key1 == val1 test FAILED\n");
        return 2;
    }

    bksmt_dict_set(dhandle, key1, val2);

    if (strcmp(bksmt_dict_get(dhandle, key1), val2) != 0) { 
        fprintf(stderr, "bksmt_dict_get key1 == val2 test FAILED\n");
        return 3;
    }

    bksmt_dict_set(dhandle, key2, val2);

    if (strcmp(bksmt_dict_get(dhandle, key2), val2) != 0) { 
        fprintf(stderr, "bksmt_dict_get key2 == val2 test FAILED\n");
        return 4;
    }

    bksmt_dict_clear(dhandle, key2);

    if (bksmt_dict_get(dhandle, key2) != NULL) { 
        fprintf(stderr, "bksmt_dict_get == NULL test FAILED\n");
        return 5;
    }

    bksmt_dict_set(d2handle, key2, val1);
    bksmt_dict_apply(dhandle, d2handle);

    if (strcmp(bksmt_dict_get(dhandle, key2), val1) != 0) { 
        fprintf(stderr, "bksmt_dict_apply key2 == val1 test FAILED\n");
        return 6;
    }

    bksmt_dict_free(dhandle);
    bksmt_dict_free(d2handle);

    fprintf(stderr, "all tests suceeded\n");
    return 0;
}
