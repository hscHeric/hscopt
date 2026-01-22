#include "hscopt/hho.h"

#include "hscopt/defs.h"

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
  double *rabbit_keys; // [dim] 

  double *mean_pos; // [dim] 
  double *tmp1;     // [dim] 
  double *tmp2;     // [dim]
  double *levy;     // [dim]

  struct {
    int has_spare;
    double spare;
  } gauss;

  double levy_sigma; 
};

/* Box–Muller com cache */
static HSCOPT_INLINE double hho_randn(hscopt_rng *rng, hscopt_hho_ctx *ctx) {
}


