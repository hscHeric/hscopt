#ifndef HSCOPT_HHO_H
#define HSCOPT_HHO_H

#include <stddef.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file hscopt_hho.h
 * @brief Harris Hawks Optimization (HHO).
 *
 *
 * @note
 * - O algoritmo assume MINIMIZAÇÃO da função objetivo.
 * - Para problemas de maximização, o decoder deve retornar o valor negado.
 * - Quando compilado com OpenMP, a avaliação da população pode ser
 * paralelizada. Nesse caso, o decoder e seu contexto devem ser thread-safe.
 */

/**
 * @struct hscopt_hho_ctx
 * @brief Contexto opaco do algoritmo HHO.
 */
typedef struct hscopt_hho_ctx hscopt_hho_ctx;

/**
 * @brief Cria e inicializa um contexto do HHO.
 *
 * Esta função:
 * - aloca o contexto e todos os buffers internos,
 * - inicializa a população de hawks com valores aleatórios em [0,1),
 * - avalia a população inicial,
 * - define a melhor solução inicial (rabbit).
 *
 * @param dim Número de chaves (dimensão do problema).
 * @param n_agents Número de agentes (hawks).
 * @param max_iters Número máximo de iterações do algoritmo (T no paper).
 * @param max_threads Número máximo de threads para avaliação (se OpenMP estiver
 * ativo).
 * @param decoder Função decoder responsável por avaliar uma solução.
 * @param dctx Contexto do decoder (pode ser NULL).
 * @param rng Gerador de números aleatórios (obrigatório).
 *
 * @return Ponteiro para o contexto HHO em caso de sucesso, ou NULL em erro.
 */
hscopt_hho_ctx *hscopt_hho_create(size_t dim, size_t n_agents,
                                  unsigned max_iters, unsigned max_threads,
                                  hscopt_decoder_fn decoder,
                                  hscopt_decode_ctx *dctx, hscopt_rng *rng);

/**
 * @brief Libera todos os recursos associados ao contexto HHO.
 *
 * Após esta chamada, o ponteiro ctx torna-se inválido.
 *
 * @param ctx Contexto HHO.
 */
void hscopt_hho_destroy(hscopt_hho_ctx *ctx);

/**
 * @brief Reinicializa o algoritmo HHO.
 *
 * Esta função:
 * - gera uma nova população aleatória de hawks,
 * - reinicia o contador de iterações,
 * - reinicia o melhor fitness encontrado.
 *
 * @param ctx Contexto HHO.
 * @return 0 em sucesso, valor diferente de 0 em erro.
 */
int hscopt_hho_reset(hscopt_hho_ctx *ctx);

/**
 * @brief Executa um número de iterações do HHO.
 *
 * Cada iteração executa, conceitualmente:
 * - avaliação dos agentes,
 * - atualização do rabbit (melhor solução),
 * - cálculo da energia de fuga,
 * - atualização das posições dos hawks conforme as fases
 *   de exploração e exploração definidas no HHO.
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
 * @brief Retorna o número máximo de iterações configurado.
 *
 * @param ctx Contexto HHO.
 * @return Número máximo de iterações (T).
 */
unsigned hscopt_hho_max_iters(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o número de agentes (hawks).
 *
 * @param ctx Contexto HHO.
 * @return Número de agentes.
 */
size_t hscopt_hho_n_agents(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna a dimensão do problema (número de chaves).
 *
 * @param ctx Contexto HHO.
 * @return Número de chaves (dimensão).
 */
size_t hscopt_hho_n_keys(const hscopt_hho_ctx *ctx);

/**
 * @brief Retorna o número máximo de threads configurado para avaliação.
 *
 * @param ctx Contexto HHO.
 * @return Número máximo de threads.
 */
unsigned hscopt_hho_max_threads(const hscopt_hho_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /* !HSCOPT_HHO_H */
