#include "decode_ctx_internal.h"

#include <stdlib.h>

unsigned hscopt_decode_ctx_effective_threads(unsigned requested_threads,
                                             const hscopt_decode_ctx *dctx) {
  if (requested_threads == 0u) {
    requested_threads = 1u;
  }

  if (!dctx || !dctx->ws || !dctx->ws_clone) {
    if (dctx && dctx->ws && requested_threads > 1u) {
      return 1u;
    }
    return requested_threads;
  }

  return requested_threads;
}

int hscopt_decode_ctx_tls_init(hscopt_decode_ctx **out_tls, size_t n_tls,
                               const hscopt_decode_ctx *src) {
  if (!out_tls || n_tls == 0u) {
    return 1;
  }

  *out_tls = NULL;
  if (!src) {
    return 0;
  }

  hscopt_decode_ctx *tls =
      (hscopt_decode_ctx *)calloc(n_tls, sizeof(hscopt_decode_ctx));
  if (!tls) {
    return 2;
  }

  for (size_t i = 0; i < n_tls; ++i) {
    tls[i] = *src;
  }

  if (src->ws && src->ws_clone) {
    for (size_t i = 1; i < n_tls; ++i) {
      tls[i].ws = src->ws_clone(src->ws, src->user);
      if (!tls[i].ws) {
        hscopt_decode_ctx_tls_destroy(tls, i, src);
        return 3;
      }
    }
  }

  *out_tls = tls;
  return 0;
}

void hscopt_decode_ctx_tls_destroy(hscopt_decode_ctx *tls, size_t n_tls,
                                   const hscopt_decode_ctx *src) {
  if (!tls) {
    return;
  }

  if (src && src->ws && src->ws_clone && src->ws_destroy) {
    for (size_t i = 1; i < n_tls; ++i) {
      if (tls[i].ws) {
        src->ws_destroy(tls[i].ws, src->user);
      }
    }
  }

  free(tls);
}
