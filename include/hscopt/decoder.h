#ifndef HSCOPT_DECODER_H
#define HSCOPT_DECODER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * hscopt_decoder.h - Decoder (Random Keys -> valor objetivo).
 *
 * A biblioteca hscopt representa uma solução por um vetor de chaves reais
 * ("random keys"). Um decoder transforma essas chaves em uma solução do
 * problema e retorna o valor da função objetivo.
 *
 * Contrato:
 * - keys aponta para um array de tamanho n_keys em [0,1).
 * - Deve ser determinístico: mesma (keys, inst, ctx) tem o mesmo retorno
 * - Não deve alocar memória no hot loop (usar ctx->ws).
 * - ctx->inst é somente leitura.
 * - Convenção: minimização. Para maximização, retornar -score.
 */

typedef struct hscopt_instance hscopt_instance;
typedef struct hscopt_workspace hscopt_workspace;

/**
 * struct hscopt_decode_ctx - Contexto passado ao decoder.
 * inst: dados do problema (somente leitura).
 * user: ponteiro opcional para dados do usuário (pode ser NULL).
 * ws: workspace/buffers reutilizáveis (recomendado; pode ser NULL).
 *
 * O contexto permite que o decoder acesse a instância e reutilize buffers
 * temporários sem alocar memória a cada chamada.
 */
typedef struct hscopt_decode_ctx {
  const hscopt_instance *inst;
  void *user;
  hscopt_workspace *ws;
} hscopt_decode_ctx;

/**
 * função hscopt_decoder_fn - Função decoder: keys -> objetivo.
 * keys: vetor de chaves (tamanho n_keys).
 * n_keys: número de chaves.
 * ctx: contexto do decoder (instância + dados extras + workspace).
 *
 * Retorna: valor da função objetivo (double).
 *
 * Requisitos:
 * - Não modificar keys.
 * - Não modificar ctx->inst.
 * - Evitar alocações e I/O no hot loop.
 */
typedef double (*hscopt_decoder_fn)(const double *keys, size_t n,
                                    hscopt_decode_ctx *ctx);

/**
 * enum hscopt_sense - Sentido do objetivo.
 * HSCOPT_MINIMIZE: menor valor é melhor.
 * HSCOPT_MAXIMIZE: maior valor é melhor.
 */
typedef enum hscopt_sense {
  HSCOPT_MINIMIZE = 0,
  HSCOPT_MAXIMIZE = 1
} hscopt_sense;

#ifdef __cplusplus
}
#endif

#endif  // !HSCOPT_DECODER_H
