#ifndef CONNS_H_
#define CONNS_H_

#include <slow5/slow5.h>
#include <slow5curl/s5curl.h>

#ifdef __cplusplus
extern "C" {
#endif

CURL *s5curl_conns_pop(conn_stack_t *conns);
int s5curl_conns_push(conn_stack_t *conns, CURL *curl);

#ifdef __cplusplus
}
#endif

#endif