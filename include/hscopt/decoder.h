#ifndef HSCOPT_DECODER_H
#define HSCOPT_DECODER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file hscopt_decoder.h
 * @brief Decoder: transforma Random Keys -> valor da função objetivo.
 *
 * A biblioteca hscopt representa uma solução por um vetor de chaves reais
 * ("random keys"). Um decoder transforma essas chaves em uma solução do
 * problema e retorna o valor da função objetivo.
 *
 * @par Contrato
 * - @p keys aponta para um array de tamanho @p n em [0,1).
 * - Deve ser determinístico: mesma (@p keys, @p ctx) => mesmo retorno.
 * - Não deve alocar memória no hot loop (usar @p ctx->ws).
 * - @p ctx->inst é somente leitura.
 *
 * @par Convenção do objetivo
 * - Por padrão, usa-se minimização.
 * - Para maximização, use @ref hscopt_sense ou retorne -score.
 */

/* forward declarations (tipos opacos) */
typedef struct hscopt_instance hscopt_instance;
typedef struct hscopt_workspace hscopt_workspace;

/**
 * @struct hscopt_decode_ctx
 * @brief Contexto passado ao decoder.
 *
 * O contexto permite que o decoder acesse a instância e reutilize buffers
 * temporários sem alocar memória a cada chamada.
 */
typedef struct hscopt_decode_ctx {
  /** Dados do problema (somente leitura). */
  const hscopt_instance *inst;

  /** Ponteiro opcional para dados do usuário (pode ser NULL). */
  void *user;

  /**
   * Workspace / buffers reutilizáveis (recomendado; pode ser NULL).
   * Deve ser usado para evitar malloc/free no hot loop.
   */
  hscopt_workspace *ws;
} hscopt_decode_ctx;

/**
 * @typedef hscopt_decoder_fn
 * @brief Assinatura de função decoder: keys -> objetivo.
 *
 * @param keys Vetor de chaves (tamanho @p n). Valores em [0,1).
 * @param n Número de chaves.
 * @param ctx Contexto do decoder (instância + dados extras + workspace).
 *
 * @return Valor da função objetivo (double).
 *
 * @note Requisitos:
 * - Não modificar @p keys.
 * - Não modificar @p ctx->inst.
 * - Evitar alocações e I/O no hot loop.
 */
typedef double (*hscopt_decoder_fn)(const double *keys, size_t n,
                                    hscopt_decode_ctx *ctx);

/**
 * @enum hscopt_sense
 * @brief Sentido do objetivo.
 */
typedef enum hscopt_sense {
  /** Menor valor é melhor. */
  HSCOPT_MINIMIZE = 0,
  /** Maior valor é melhor. */
  HSCOPT_MAXIMIZE = 1
} hscopt_sense;

#ifdef __cplusplus
}
#endif

#endif /* HSCOPT_DECODER_H */
