#include "../../http/uri.h"
#include <string.h> 
#include <stdlib.h> 

int
main(void)
{
    /* test that all special chars expand without interfering with eachother */
    char *str1 = " !\"#$%&\'()*+,/:;=?@[]"; 
    char *pctstr1 = "%20%21%22%23%24%25%26%27%28%29%2A%2B%2C%2F%3A%3B%3D%3F%40%5B%5D"; 
    /* test that non-special chars are preserved */
    char *str2 = "abc"; 
    /* test that special char processing does not interfere with non-special chars */
    char *str3 = "abc de"; 
    char *pctstr3 = "abc%20de"; 
 
    char *sto;

    sto = bksmt_cstrpctenc(str1);
    
    if (strcmp(pctstr1, sto) != 0)
        exit(1);

    free(sto);

    sto = bksmt_cstrpctenc(str2);

    if (strcmp(str2, sto) != 0)
        exit(2);

    free(sto);

    sto = bksmt_cstrpctenc(str3);

    if (strcmp(pctstr3, sto) != 0)
        exit(3);

    free(sto);


    return 0;
}


