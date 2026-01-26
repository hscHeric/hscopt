#ifndef HSCOPT_RVNS_H
#define HSCOPT_RVNS_H

#include <stddef.h>

#include "hscopt/decoder.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct hscopt_rvns_ctx hscopt_rvns_ctx;

hscopt_rvns_ctx *hscopt_hho_create(double *x0, size_t n, size_t k,
                                   hscopt_decoder_fn decoder,
                                   hscopt_decode_ctx *dctx);

void hscopt_hho_destroy(hscopt_rvns_ctx *ctx);

int hscopt_hho_iterate(hscopt_rvns_ctx *ctx, unsigned iters);

#ifdef __cplusplus
}
#endif

#endif  // !HSCOPT_RVNS_H
