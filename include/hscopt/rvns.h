#ifndef HSCOPT_RVNS_H
#define HSCOPT_RVNS_H

#include <stddef.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hscopt_rvns_ctx hscopt_rvns_ctx;

hscopt_rvns_ctx *hscopt_rvns_create(const double *x0, size_t dim, size_t k_max,
                                    unsigned max_iters, unsigned max_threads,
                                    hscopt_decoder_fn decoder,
                                    hscopt_decode_ctx *dctx, hscopt_rng *rng);

void hscopt_rvns_destroy(hscopt_rvns_ctx *ctx);

int hscopt_rvns_reset(hscopt_rvns_ctx *ctx, const double *x0);

int hscopt_rvns_iterate(hscopt_rvns_ctx *ctx, unsigned iters);

double hscopt_rvns_best_fitness(const hscopt_rvns_ctx *ctx);

const double *hscopt_rvns_best_keys(const hscopt_rvns_ctx *ctx);

unsigned hscopt_rvns_iteration(const hscopt_rvns_ctx *ctx);

size_t hscopt_rvns_dim(const hscopt_rvns_ctx *ctx);

size_t hscopt_rvns_k_max(const hscopt_rvns_ctx *ctx);

unsigned hscopt_rvns_max_threads(const hscopt_rvns_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif  // !HSCOPT_RVNS_H
