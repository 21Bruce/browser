#ifndef __BKSMT_HTTP_RESPONSE_H__
#define __BKSMT_HTTP_RESPONSE_H__


#include <stdlib.h>

struct bksmt_http_status_lut_entry {
    char *code;
    char *stat_msg;
    size_t msglen;
} bksmt_http_status_lut[94] = {
#define HTTP_100 0
    {"100", "Continue"                       , 8 },
#define HTTP_101 1
    {"101", "Switching Protocols"            , 19},
#define HTTP_102 2
    {"102", "Processing"                     , 10},
#define HTTP_103 3
    {"103", "Early Hints"                    , 11},
#define HTTP_200 4
    {"200", "OK"                             , 2 },
#define HTTP_201 5
    {"201", "Created"                        , 7 },
#define HTTP_202 6
    {"202", "Accepted"                       , 8 },
#define HTTP_203 7
    {"203", "Non-Authoritative Information"  , 29},
#define HTTP_204 8
    {"204", "No Content"                     , 10},
#define HTTP_205 9
    {"205", "Reset Content"                  , 13},
#define HTTP_206 10 
    {"206", "Partial Content"                , 15},
#define HTTP_207 11 
    {"207", "Multi-Status"                   , 12},
#define HTTP_208 12 
    {"208", "Already Reported"               , 16},
#define HTTP_226 13 
    {"226", "IM Used"                        , 7 },
#define HTTP_300 14 
    {"300", "Multiple Choices"               , 16},
#define HTTP_301 15 
    {"301", "Moved Permanently"              , 17},
#define HTTP_302 16 
    {"302", "Found"                          , 5 },
#define HTTP_303 17 
    {"303", "See Other"                      , 9 },
#define HTTP_304 18 
    {"304", "Not Modified"                   , 12},
#define HTTP_305 19 
    {"305", "Use Proxy"                      , 9 },
#define HTTP_306 20 
    {"306", "unused"                         , 6 },
#define HTTP_307 21 
    {"307", "Temporary Redirect"             , 18},
#define HTTP_308 22 
    {"308", "Permanent Redirect"             , 18},
#define HTTP_400 23 
    {"400", "Bad Request"                    , 11},
#define HTTP_401 24 
    {"401", "Unauthorized"                   , 12},
#define HTTP_402 25 
    {"402", "Payment Required"               , 16},
#define HTTP_403 26 
    {"403", "Forbidden"                      , 9 },
#define HTTP_404 27 
    {"404", "Not Found"                      , 9 },
#define HTTP_405 28 
    {"405", "Method Not Allowed"             , 18},
#define HTTP_406 29 
    {"406", "Not Acceptable"                 , 14},
#define HTTP_407 30 
    {"407", "Proxy Authentication Required"  , 29},
#define HTTP_408 31 
    {"408", "Request Timeout"                , 15},
#define HTTP_409 32 
    {"409", "Conflict"                       , 8 },
#define HTTP_410 33 
    {"410", "Gone"                           , 4 },
#define HTTP_411 34 
    {"411", "Length Required"                , 15},
#define HTTP_412 35 
    {"412", "Precondition Failed"            , 19},
#define HTTP_413 36 
    {"413", "Payload Too Large"              , 17},
#define HTTP_414 37 
    {"414", "URI Too Long"                   , 12},
#define HTTP_415 38 
    {"415", "Unsupported Media Type"         , 22},
#define HTTP_416 39 
    {"416", "Range Not Satisfiable"          , 23},
#define HTTP_417 40 
    {"417", "Expectation Failed"             , 20},
#define HTTP_418 41 
    {"418", "I'm a teapot"                   , 12},
#define HTTP_421 42 
    {"421", "Misdirected Request"            , 19},
#define HTTP_422 43 
    {"422", "Unprocessable Content"          , 21},
#define HTTP_423 44 
    {"423", "Locked"                         , 6 },
#define HTTP_424 45 
    {"424", "Failed Dependency"              , 17},
#define HTTP_425 46 
    {"425", "Too Early"                      , 9 },
#define HTTP_426 47 
    {"426", "Upgrade Required"               , 16},
#define HTTP_428 48 
    {"428", "Precondition Required"          , 21},
#define HTTP_429 49 
    {"429", "Too Many Requests"              , 17},
#define HTTP_431 50 
    {"431", "Request Header Fields Too Large", 31},
#define HTTP_451 51 
    {"451", "Unavailable For Legal Reasons"  , 29},
#define HTTP_500 52 
    {"500", "Internal Server Error"          , 21},
#define HTTP_501 53 
    {"501", "Not Implemented"                , 15},
#define HTTP_502 54 
    {"502", "Bad Gateway"                    , 11},
#define HTTP_503 55 
    {"503", "Service Unavailable"            , 19},
#define HTTP_504 56 
    {"504", "Gateway Timeout"                , 15},
#define HTTP_505 57 
    {"505", "HTTP Version Not Supported"     , 26},
#define HTTP_506 58 
    {"506", "Variant Also Negotiates"        , 23},
#define HTTP_507 59 
    {"507", "Insufficient Storage"           , 20},
#define HTTP_508 60 
    {"508", "Loop Detected"                  , 13},
#define HTTP_510 61 
    {"510", "Not Extended"                   , 12},
#define HTTP_511 62 
    {"511", "Network Authentication Required", 31},
}
