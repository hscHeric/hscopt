#include "hscopt/hho.h"

#include <math.h>
#include <stdlib.h>

// índice linear agente -> dimensão
#define HHO_IDX(ctx, agent, d) ((agent) * (ctx)->dim + (d))

// ponteiro para o vetor de chaves (posição) do agente
#define HHO_AGENT_PTR(ctx, agent) (&(ctx)->X[(agent) * (ctx)->dim])

// acesso a um componente específico X[agent][d]
#define HHO_X(ctx, agent, d) ((ctx)->X[HHO_IDX((ctx), (agent), (d))])

struct hscopt_hho_ctx {
  size_t dim;       // dimensão do problema
  size_t n_agents;  // numero de gavioes
  size_t iter;      // iteração atual

  hscopt_decoder_fn decoder;
  hscopt_decode_ctx *dctx;
  hscopt_rng *rng;

  double *X;              // gaviões [dim * n_agents]
  double *fitness;        // buffer para fitness de tamanho n_agents
  double rabbit_fitness;  // melhor fitness encontrado
  double *rabbit_keys;    // melhor posição encontrada

  double *mean_pos;  // posição média do enxame
  double *tmp1;      // temporario
  double *tmp2;
  double *levy;  // buffer de retorno do levy
};

hscopt_hho_ctx *hscopt_hho_create(size_t dim, size_t n_agents,
                                  hscopt_decoder_fn decoder,
                                  hscopt_decode_ctx *dctx, hscopt_rng *rng) {
  // Verificação dos parâmetros
  if (!decoder || !rng || dim == 0 || n_agents == 0) {
    return NULL;
  }

  hscopt_hho_ctx *ctx = (hscopt_hho_ctx *)calloc(1, sizeof(*ctx));
  if (!ctx) {
    return NULL;
  }

  ctx->dim = dim;
  ctx->n_agents = n_agents;
  ctx->iter = 0;
  ctx->dctx = dctx;
  ctx->rng = rng;
  ctx->rabbit_fitness = INFINITY;

  // Alocação dos buffers
  ctx->X = (double *)malloc(sizeof(double) * (dim * n_agents));
  ctx->fitness = (double *)malloc(sizeof(double) * n_agents);
  ctx->rabbit_keys = (double *)malloc(sizeof(double) * dim);
  ctx->mean_pos = (double *)malloc(sizeof(double) * dim);
  ctx->tmp1 = (double *)malloc(sizeof(double) * dim);
  ctx->tmp2 = (double *)malloc(sizeof(double) * dim);
  ctx->levy = (double *)malloc(sizeof(double) * dim);

  if (!ctx->X || !ctx->fitness || !ctx->rabbit_keys || !ctx->mean_pos ||
      !ctx->tmp1 || !ctx->tmp2 || !ctx->levy) {
    return NULL;
  }

  return ctx;
}
