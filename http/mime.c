/*
 * copyright(c) bruce jagid, november 2023
 */

#include "./mime.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static int 
islower(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;

    return 0;
}

static char 
toupper(char c)
{
    assert(c - 'a' <= 25);
    
    return 'A' + c - 'a';
}

/*
 * bksmt_cstrmime - converts string to canonical 
 * MIME form
 * Example: "accept-encoding" -> "Accept-Encoding"
 *
 * => modifies str in place
 */
void
bksmt_cstrmime(char *str)
{
    int sf = 1;
    size_t i;

    for(i = 0; str[i] != 0; i++) {
        if (str[i] == '-') {
            sf = 1;     
            continue;
        } else if (sf && islower(str[i])) {
            str[i] = toupper(str[i]);
        }
        sf = 0;
    }
}

