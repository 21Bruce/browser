#ifndef __BKSMT_STRCONV_H__
#define __BKSMT_STRCONV_H__

char *inttocstr(int);
int   cstrtoint(char *);
int   csubstrtoint(char *, char *);
int   hexcstrtoint(char *);
int   hexcsubstrtoint(char *, char *);

#endif
