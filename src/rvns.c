#include "hscopt/rvns.h"

#include <math.h>
#include <stddef.h>

#include "hscopt/decoder.h"

#define CAND_PTR(ctx, tid) (&(ctx)->cand_keys[(size_t)(tid) * (ctx)->dim])

struct hscopt_rvns_ctx {
  size_t dim;                 // tamanho do vetor de chaves aleatórias
  size_t k_max;               // pertubação máxima
  unsigned iter;              // iteração atual
  unsigned max_iters;         // número máximo de iterações
  unsigned max_threads;       // número máximo de threads
  unsigned eff_threads;       // número real de threads usadas
  hscopt_decoder_fn decoder;  // decoder
  hscopt_decode_ctx dctx;     // contexto do decder
  hscopt_rng *rng_tls;        // vetor de rng[eff_threads]
  double *x;                  // melhor atual
  double fx;                  // melhor função objetivo
  double *best;               // melhor global
  double fbest;               // função objetivo do melhor global
  double *cand_keys;          // candidatos de tamanho [dim * eff_threads]
  double *cand_fit;           // candidatos de tamanho [eff_threads]
};

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
