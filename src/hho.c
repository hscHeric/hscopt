#include "hscopt/hho.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hscopt/defs.h"
#include "hscopt/rng.h"

#define LEVY_SIGMA 1.5

/* ponteiro para o agente i (vetor de dim doubles) */
#define HAWK_PTR(ctx, agent) (&(ctx)->X[(agent) * (ctx)->dim])

/* t e T do paper: E1 = 2*(1 - t/T) */
#define E1(t, T) (2.0 * (1.0 - ((double)(t) / (double)(T))))

/* E0 em (-1,1): 2*u - 1 */
#define E0(u01) (2.0 * (u01) - 1.0)

/* clamp para random keys em [0,1) */
#define HHO_CLAMP_KEY(x) HSCOPT_CLAMP((x), 0.0, (1.0 - 1e-15))

struct hscopt_hho_ctx {
  size_t dim;
  size_t n_agents;

  unsigned iter;       // t
  unsigned max_iters;  // T
  unsigned max_threads;

  hscopt_decoder_fn decoder;
  hscopt_decode_ctx *dctx;
  hscopt_rng *rng;

  double *X;        // [n_agents * dim]
  double *fitness;  // [n_agents]

  double rabbit_fitness;
  double *rabbit_keys;  // [dim]

  double *mean_pos;  // [dim]
  double *tmp1;      // [dim]
  double *tmp2;      // [dim]
  double *levy;      // [dim]

  struct {
    int has_spare;
    double spare;
  } gauss;

  double levy_sigma;
};

/* Box–Muller com cache */
HSCOPT_INLINE double hho_randn(hscopt_rng *rng, hscopt_hho_ctx *ctx) {
  if (ctx->gauss.has_spare) {
    ctx->gauss.has_spare = 0;
    return ctx->gauss.spare;
  }

  double u1 = hscopt_rng_next_u01(rng);
  double u2 = hscopt_rng_next_u01(rng);
  if (u1 <= 0.0) {
    u1 = 1e-12;
  }

  const double r = sqrt(-2.0 * log(u1));
  const double theta = 2.0 * HSCOPT_PI * u2;

  ctx->gauss.spare = r * sin(theta);
  ctx->gauss.has_spare = 1;
  return r * cos(theta);
}

hscopt_hho_ctx *hscopt_hho_create(size_t dim, size_t n_agents,
                                  unsigned max_iters, unsigned max_threads,
                                  hscopt_decoder_fn decoder,
                                  hscopt_decode_ctx *dctx, hscopt_rng *rng) {
  if (!decoder || !rng || dim == 0 || n_agents == 0 || max_iters == 0) {
    return NULL;
  }

  hscopt_hho_ctx *ctx = (hscopt_hho_ctx *)calloc(1, sizeof(*ctx));
  if (!ctx) {
    return NULL;
  }

  ctx->dim = dim;
  ctx->n_agents = n_agents;
  ctx->iter = 0;
  ctx->max_iters = max_iters;
  ctx->max_threads = max_threads;

  ctx->decoder = decoder;
  ctx->dctx = dctx;
  ctx->rng = rng;

  ctx->X = (double *)malloc(sizeof(double) * (dim * n_agents));
  ctx->fitness = (double *)malloc(sizeof(double) * n_agents);
  ctx->rabbit_keys = (double *)malloc(sizeof(double) * dim);
  ctx->mean_pos = (double *)malloc(sizeof(double) * dim);
  ctx->tmp1 = (double *)malloc(sizeof(double) * dim);
  ctx->tmp2 = (double *)malloc(sizeof(double) * dim);
  ctx->levy = (double *)malloc(sizeof(double) * dim);

  if (!ctx->X || !ctx->fitness || !ctx->rabbit_keys || !ctx->mean_pos ||
      !ctx->tmp1 || !ctx->tmp2 || !ctx->levy) {
    hscopt_hho_destroy(ctx);
    return NULL;
  }

  // Cálculo do levy_sigma
  const double beta = 1.5;
  const double num = tgamma(1.0 + beta) * sin(HSCOPT_PI * beta / 2.0);
  const double den =
      tgamma((1.0 + beta) / 2.0) * beta * pow(2.0, (beta - 1.0) / 2.0);

  ctx->levy_sigma = pow(num / den, 1.0 / beta);
  ctx->gauss.has_spare = 0;
  ctx->gauss.spare = 0.0;

  if (hscopt_hho_reset(ctx) != 0) {
    hscopt_hho_destroy(ctx);
    return NULL;
  }

  return ctx;
}

void hscopt_hho_destroy(hscopt_hho_ctx *ctx) {
  if (!ctx) return;
  free(ctx->X);
  free(ctx->fitness);
  free(ctx->rabbit_keys);
  free(ctx->mean_pos);
  free(ctx->tmp1);
  free(ctx->tmp2);
  free(ctx->levy);
  free(ctx);
}

HSCOPT_INLINE void hho_eval_all(hscopt_hho_ctx *ctx) {
#ifdef _OPENMP
#pragma omp parallel for num_threads(ctx->num_threads)
#endif
  for (ptrdiff_t i = 0; i < (ptrdiff_t)ctx->n_agents; ++i) {
    double *x = HAWK_PTR(ctx, (size_t)i);
    ctx->fitness[(size_t)i] = ctx->decoder(x, ctx->dim, ctx->dctx);
  }

  for (size_t i = 0; i < ctx->n_agents; ++i) {
    if (ctx->fitness[i] < ctx->rabbit_fitness) {
      ctx->rabbit_fitness = ctx->fitness[i];
      memcpy(ctx->rabbit_keys, HAWK_PTR(ctx, i), ctx->dim * sizeof(double));
    }
  }
}

int hscopt_hho_reset(hscopt_hho_ctx *ctx) {
  if (!ctx) {
    return 1;
  }

  ctx->iter = 0;
  ctx->rabbit_fitness = INFINITY;
  ctx->gauss.has_spare = 0;
  ctx->gauss.spare = 0;

  memset(ctx->rabbit_keys, 0, ctx->dim * sizeof(double));

  // inicializa os gaviões
  for (size_t i = 0; i < ctx->n_agents; ++i) {
    double *x = HAWK_PTR(ctx, i);
    for (size_t j = 0; j < ctx->dim; j++) {
      x[j] = hscopt_rng_next_u01(ctx->rng);
    }
  }

  // calcula o fitness dos gaviões
  hho_eval_all(ctx);
  return 0;
}

HSCOPT_INLINE void hho_clamp_vec(double *x, size_t n) {
  for (size_t i = 0; i < n; ++i) x[i] = HHO_CLAMP_KEY(x[i]);
}

HSCOPT_INLINE void hho_mean_pos(hscopt_hho_ctx *ctx) {
  memset(ctx->mean_pos, 0, ctx->dim * sizeof(double));
  for (size_t i = 0; i < ctx->n_agents; ++i) {
    double *x = HAWK_PTR(ctx, i);
    for (size_t j = 0; j < ctx->dim; ++j) ctx->mean_pos[j] += x[j];
  }
  const double inv = 1.0 / (double)ctx->n_agents;
  for (size_t j = 0; j < ctx->dim; ++j) ctx->mean_pos[j] *= inv;
}

int hscopt_hho_iterate(hscopt_hho_ctx *ctx, unsigned int iters) {
  if (!ctx || iters == 0) {
    return 1;
  }
  if (ctx->iter >= ctx->max_iters || ctx->iter + iters > ctx->max_iters) {
    return 2;
  }

  // loop principal de iterações
  for (size_t it = 0; it < iters; ++iters) {
    // clamp e eval dos gavioes para garantir que estão no hipercubo
    for (size_t i = 0; i < ctx->n_agents; ++i) {
      hho_clamp_vec(HAWK_PTR(ctx, i), ctx->dim);
    }
  }
  hho_eval_all(ctx);

  const double E1 = E1(ctx->iter, ctx->max_iters);
  hho_mean_pos(ctx);  // calcula a pocição media do enxame

  for (size_t i = 0; i < ctx->n_agents; ++i) {
    double *Xi = HAWK_PTR(ctx, i);
    const double E0 = E0(hscopt_rng_next_u01(ctx->rng));
    const double E = E1 * E0;

    if (fabs(E) >= 1.0) {
      // q aleatorio que define a estratégia de exploração
      const double q = hscopt_rng_next_u01(ctx->rng);

      // Calculando falcao aleatorio
      size_t r_idx =
          (size_t)(hscopt_rng_next_u01(ctx->rng) * (double)ctx->n_agents);
      if (r_idx >= ctx->n_agents) r_idx = ctx->n_agents - 1;
      double *Xrand = HAWK_PTR(ctx, r_idx);

      // Equação  (1) do A brief description of the HHO algorithm, está no drive
      if (q >= 0.5) {
        const double r1 = hscopt_rng_next_u01(ctx->rng);
        const double r2 = hscopt_rng_next_u01(ctx->rng);
        for (size_t j = 0; j < ctx->dim; ++j) {
          const double val = Xrand[j] - r1 * fabs(Xrand[j] - 2.0 * r2 * Xi[j]);
          Xi[j] = HHO_CLAMP_KEY(val);
        }
      } else {
        const double r3 = hscopt_rng_next_u01(ctx->rng);
        const double r4 = hscopt_rng_next_u01(ctx->rng);
        const double s = r3 * r4;
        for (size_t j = 0; j < ctx->dim; ++j) {
          const double val = (ctx->rabbit_keys[j] - ctx->mean_pos[j]) - s;
          Xi[j] = HHO_CLAMP_KEY(val);
        }
      }
    } else {
      // TODO: fase de intensificação
    }
  }

  return 1;
}
