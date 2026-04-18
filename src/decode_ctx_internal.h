#ifndef HSCOPT_DECODE_CTX_INTERNAL_H
#define HSCOPT_DECODE_CTX_INTERNAL_H

#include <stddef.h>

#include "hscopt/decoder.h"

unsigned hscopt_decode_ctx_effective_threads(unsigned requested_threads,
                                             const hscopt_decode_ctx *dctx);

int hscopt_decode_ctx_tls_init(hscopt_decode_ctx **out_tls, size_t n_tls,
                               const hscopt_decode_ctx *src);

void hscopt_decode_ctx_tls_destroy(hscopt_decode_ctx *tls, size_t n_tls,
                                   const hscopt_decode_ctx *src);

#endif /* HSCOPT_DECODE_CTX_INTERNAL_H */
