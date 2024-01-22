#ifndef __BKSMT_HTTP_H__
#define __BKSMT_HTTP_H__

#define CRLF "\r\n"

#define CR '\r'

#define LF '\n'

/*
 * LUT for fast protocol type to str conversion
 */
static struct bksmt_http_prot_lut_entry{
    char *prot;
    int   len;
} bksmt_http_prot_lut[2] = {
#define HTTP_HTTP  0
    { "http", 4 },
#define HTTP_HTTPS 1
    { "https", 5 },
};

/* http request status codes */
#define HTTP_OK     0
#define HTTP_ERROR  1
#define HTTP_BUSY   2

#endif
