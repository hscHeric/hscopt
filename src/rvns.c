#include "hscopt/rvns.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#define CAND_PTR(ctx, tid) (&(ctx)->cand_keys[(size_t)(tid) * (ctx)->dim])

struct hscopt_rvns_ctx {
  size_t dim;                 // tamanho do vetor de chaves aleatórias
  size_t k_max;               // pertubação máxima
  unsigned iter;              // iteração atual
  unsigned max_iters;         // número máximo de iterações
  unsigned max_threads;       // número máximo de threads
  unsigned eff_threads;       // número real de threads usadas
  hscopt_decoder_fn decoder;  // decoder
  hscopt_decode_ctx *dctx;    // contexto do decder
  hscopt_rng *rng_tls;        // vetor de rng[eff_threads]
  double *x;                  // melhor atual
  double fx;                  // melhor função objetivo
  double *best;               // melhor global
  double fbest;               // função objetivo do melhor global
  double *cand_keys;          // candidatos de tamanho [dim * eff_threads]
  double *cand_fit;           // candidatos de tamanho [eff_threads]
};

int hscopt_rvns_reset(hscopt_rvns_ctx *ctx, const double *x0) {
  if (!ctx) {
    return 1;  // erro ctx null
  }

  ctx->iter = 0;
  if (x0) {
    memcpy(ctx->x, x0, ctx->dim * sizeof(double));
    for (size_t i = 0; i < ctx->dim; ++i) {
      ctx->x[i] = HSCOPT_CLAMP_KEY(ctx->x[i]);
    }
  } else {
    for (size_t i = 0; i < ctx->dim; ++i) {
      ctx->x[i] = hscopt_rng_next_u01(&ctx->rng_tls[0]);
    }
  }

  ctx->fx = ctx->decoder(ctx->x, ctx->dim, ctx->dctx);
  memcpy(ctx->best, ctx->x, ctx->dim * sizeof(double));
  ctx->fbest = ctx->fx;

  return 0;
}

void hscopt_rvns_destroy(hscopt_rvns_ctx *ctx) {
  if (!ctx) return;
  free(ctx->rng_tls);
  free(ctx->x);
  free(ctx->best);
  free(ctx->cand_keys);
  free(ctx->cand_fit);
  free(ctx);
}

hscopt_rvns_ctx *hscopt_rvns_create(const double *x0, size_t dim, size_t k_max,
                                    unsigned max_iters, unsigned max_threads,
                                    hscopt_decoder_fn decoder,
                                    hscopt_decode_ctx *dctx, hscopt_rng *rng) {
  if (!decoder || !rng || max_iters == 0 || k_max == 0 || dim == 0) {
    return NULL;
  }

  hscopt_rvns_ctx *ctx = (hscopt_rvns_ctx *)calloc(1, sizeof(*ctx));
  if (!ctx) {
    return NULL;
  };

  ctx->dim = dim;
  ctx->k_max = k_max;
  ctx->iter = 0;
  ctx->max_iters = max_iters;
  ctx->max_threads = (max_threads == 0u ? 1u : max_threads);

#ifdef _OPENMP
  ctx->eff_threads = (max_threads == 0u ? 1u : max_threads);
#else
  ctx->eff_threads = 1u;
#endif /* ifdef _OPENMP */

  ctx->decoder = decoder;
  ctx->dctx = dctx;
  ctx->x = (double *)malloc(dim * sizeof(double));
  ctx->best = (double *)malloc(dim * sizeof(double));
  ctx->cand_keys =
      (double *)malloc((size_t)ctx->eff_threads * dim * sizeof(double));
  ctx->cand_fit = (double *)malloc((size_t)ctx->eff_threads * sizeof(double));

  if (!ctx->x || !ctx->best || !ctx->cand_keys || !ctx->cand_fit) {
    hscopt_rvns_destroy(ctx);
    return NULL;
  }

  // alocação das instâncias de RNG
  ctx->rng_tls =
      (hscopt_rng *)calloc((size_t)ctx->eff_threads, sizeof(hscopt_rng));
  if (!ctx->rng_tls) {
    hscopt_rvns_destroy(ctx);
    return NULL;
  }

  ctx->rng_tls[0] = *rng;
  for (unsigned i = 1; i < ctx->eff_threads; ++i) {
    ctx->rng_tls[i] = ctx->rng_tls[i - 1];
    hscopt_rng_long_jump(&ctx->rng_tls[i]);
  }

  if (hscopt_rvns_reset(ctx, x0) != 0) {
    hscopt_rvns_destroy(ctx);
    return NULL;
  }

  return ctx;
}

double hscopt_rvns_best_fitness(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->fbest : INFINITY;
}

const double *hscopt_rvns_best_keys(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->best : NULL;
}

unsigned hscopt_rvns_iteration(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->iter : 0u;
}

size_t hscopt_rvns_dim(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->dim : 0u;
}

size_t hscopt_rvns_k_max(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->k_max : 0u;
}

unsigned hscopt_rvns_max_threads(const hscopt_rvns_ctx *ctx) {
  return ctx ? ctx->eff_threads : 1u;
}
