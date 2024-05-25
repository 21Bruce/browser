#include "../../lib/strconv.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int
main(void) 
{
    int ibox = 123;    
    char *sbox;
    char sbase[4] = "123";
    char hbase[4] = "A1C";

    sbox = inttocstr(ibox);
    if (strcmp(sbox, sbase) != 0) {
        fprintf(stderr, "inttocstr test FAILED\n");
        return 1;
    }
    free(sbox);

    ibox = cstrtoint(sbase);
    if (ibox != 123) {
        fprintf(stderr, "cstrtoint test FAILED\n");
        return 2;
    }

    ibox = csubstrtoint(sbase + 1, sbase + 2);
    if (ibox != 2) {
        fprintf(stderr, "csubstrtoint test FAILED\n");
        return 3;
    }

    ibox = hexcstrtoint(hbase);
    if (ibox != 2588) {
        fprintf(stderr, "hexcstrtoint test FAILED\n");
        return 4;
    }

    ibox = hexcsubstrtoint(hbase, hbase + 2);
    if (ibox != 161) {
        fprintf(stderr, "hexcsubstrtoint test FAILED\n");
        return 5;
    }
   
    fprintf(stderr, "all tests suceeded");
    return 0;
}
