#include <clt13.h>
#include "util_clt.h"

void util_clt_state_store(FILE *mmap_f, clt_state_t *state)
{
    if (clt_state_fwrite(state, mmap_f) == CLT_ERR)
    {
        fprintf(stderr, "clt_state_fwrite failed!\n");
        exit(1);
    }
}

clt_state_t *util_clt_state_restore(FILE *mmap_f)
{
    clt_state_t *state;

    if ((state = clt_state_fread(mmap_f)) == NULL)
    {
        fprintf(stderr, "clt_state_fread failed!\n");
        exit(1);
    }

    return state;
}

void util_clt_pp_store(FILE *mmap_f, clt_pp_t *pp)
{
    if (clt_pp_fwrite(pp, mmap_f) == CLT_ERR)
    {
        fprintf(stderr, "clt_pp_fwrite failed!\n");
        exit(1);
    }
}

clt_pp_t *util_clt_pp_restore(FILE *mmap_f)
{
    clt_pp_t *pp;

    if ((pp = clt_pp_fread(mmap_f)) == NULL)
    {
        fprintf(stderr, "clt_pp_fread failed!\n");
        exit(1);
    }

    return pp;
}

void util_clt_elem_store(FILE *mmap_f, clt_elem_t *elem)
{
    if (clt_elem_fwrite(elem, mmap_f) == CLT_ERR)
    {
        fprintf(stderr, "clt_elem_fwrite failed!\n");
        exit(1);
    }
}

void util_clt_elem_restore(FILE *mmap_f, clt_elem_t *elem)
{
    if (clt_elem_fread(elem, mmap_f) == CLT_ERR)
    {
        fprintf(stderr, "clt_elem_fread failed!\n");
        exit(1);
    }
}
