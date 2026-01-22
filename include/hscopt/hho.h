#ifndef HSCOPT_HHO_H
#define HSCOPT_HHO_H

#include <stddef.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file hho.h
 * @brief Harris Hawks Optimization (HHO).
 *
 * Implementa o algoritmo Harris Hawks Optimization (HHO) usando representação
 * por Random Keys.
 *
 * @note
 * - O algoritmo assume MINIMIZAÇÃO da função objetivo.
 */

/**
 * @struct hscopt_hho_ctx
 * @brief Contexto opaco do algoritmo HHO.
 *
 * O contexto encapsula:
 * - parâmetros do algoritmo
 * - estado atual da população de hawks,
 * - melhor solução encontrada ,
 * - contador de iterações,
 * - referências ao RNG e ao decoder.
 *
 * O usuário não deve acessar nem modificar diretamente os campos internos.
 */
typedef struct hscopt_hho_ctx hscopt_hho_ctx;

/**
 * @brief Cria e inicializa um contexto HHO.
 *
 * Aloca os buffers internos e inicializa a população de agentes
 * (hawks) com chaves aleatórias em [0,1).
 *
 * @param n Número de chaves (dimensão do problema).
 * @param N Número de agentes (hawks) na população.
 * @param decoder Função decoder (random keys -> valor da função objetivo).
 * @param dctx Contexto do decoder (pode ser NULL).
 * @param rng Gerador de números aleatórios (obrigatório).
 *
 * @return Ponteiro para o contexto HHO ou NULL em caso de erro.
 */
hscopt_hho_ctx *hscopt_hho_create(size_t n, size_t N, hscopt_decoder_fn decoder,
                                  hscopt_decode_ctx *dctx, hscopt_rng *rng);

/**
 * @brief Libera todos os recursos associados ao contexto HHO.
 *
 * @param ctx Contexto HHO.
 */
void hscopt_hho_destroy(hscopt_hho_ctx *ctx);

/**
 * @brief Reinicializa o algoritmo HHO.
 *
 * Gera uma nova população aleatória de hawks e reinicia
 * o contador de iterações e o melhor fitness.
 *
 * @param ctx Contexto HHO.
 * @return 0 em sucesso, valor diferente de 0 em erro.
 */
int hscopt_hho_reset(hscopt_hho_ctx *ctx);

/**
 * @brief Executa um número de iterações do HHO.
 *
 * Cada iteração corresponde a:
 * - avaliação dos agentes,
 * - atualização da melhor solução (rabbit),
 * - cálculo da energia de fuga,
 * - atualização das posições dos hawks conforme as regras do HHO.
 *
 * @param ctx Contexto HHO.
 * @param iters Número de iterações a executar (>= 1).
 *
 * @return 0 em sucesso, valor diferente de 0 em erro.
 */
int hscopt_hho_iterate(hscopt_hho_ctx *ctx, unsigned iters);

/**
 * @brief Retorna o melhor valor da função objetivo encontrado.
 *
 * @param ctx Contexto HHO.
 * @return Melhor fitness encontrado até o momento.
 */
double hscopt_hho_best_fitness(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o vetor de chaves da melhor solução encontrada.
 *
 * @param ctx Contexto HHO.
 * @return Ponteiro para o vetor de random keys do melhor agente.
 *
 * @note
 * - O ponteiro retornado é válido enquanto o contexto existir
 *   e não for chamada hscopt_hho_reset() ou hscopt_hho_iterate().
 */
const double *hscopt_hho_best_keys(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o número da iteração atual.
 *
 * @param ctx Contexto HHO.
 * @return Índice da iteração atual (inicia em 0).
 */
unsigned hscopt_hho_iteration(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o número de agentes (hawks).
 *
 * @param ctx Contexto HHO.
 * @return Número de agentes.
 */
size_t hscopt_hho_n_agents(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o número de agentes (hawks).
 *
 * @param ctx Contexto HHO.
 * @return Número de agentes.
 */
size_t hscopt_hho_n_keys(const hscopt_hho_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif  // !HSCOPT_HHO_H
