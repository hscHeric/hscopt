#ifndef HSCOPT_HHO_H
#define HSCOPT_HHO_H

#include <stddef.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hscopt_hho_ctx hscopt_hho_ctx;

hscopt_hho_ctx *hscopt_hho_create(size_t dim, size_t n_agents,
                                  unsigned max_iters, unsigned max_threads,
                                  hscopt_decoder_fn decoder,
                                  hscopt_decode_ctx *dctx, hscopt_rng *rng);

void hscopt_hho_destroy(hscopt_hho_ctx *ctx);

int hscopt_hho_reset(hscopt_hho_ctx *ctx);

int hscopt_hho_iterate(hscopt_hho_ctx *ctx, unsigned iters);

double hscopt_hho_best_fitness(const hscopt_hho_ctx *ctx);

const double *hscopt_hho_best_keys(const hscopt_hho_ctx *ctx);

unsigned hscopt_hho_iteration(const hscopt_hho_ctx *ctx);

unsigned hscopt_hho_max_iters(const hscopt_hho_ctx *ctx);

size_t hscopt_hho_n_agents(const hscopt_hho_ctx *ctx);

size_t hscopt_hho_n_keys(const hscopt_hho_ctx *ctx);

unsigned hscopt_hho_max_threads(const hscopt_hho_ctx *ctx);

int hscopt_hho_try_update_rabbit(hscopt_hho_ctx *ctx, const double *keys);

#ifdef __cplusplus
}
#endif

#endif /* !HSCOPT_HHO_H */
