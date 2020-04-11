#include "clt13.h"

#ifndef UTIL_CLT_H_
#define UTIL_CLT_H_

void util_clt_state_store(FILE *mmap_f, clt_state_t *state);

clt_state_t *util_clt_state_restore(FILE *mmap_f);

void util_clt_pp_store(FILE *mmap_f, clt_pp_t *pp);

clt_pp_t *util_clt_pp_restore(FILE *mmap_f);

void util_clt_elem_store(FILE *mmap_f, clt_elem_t *elem);

void util_clt_elem_restore(FILE *mmap_f, clt_elem_t *elem);

#endif /* UTIL_CLT_H_ */
