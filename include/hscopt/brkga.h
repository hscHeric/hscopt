#ifndef HSCOPT_BRKGA_H
#define HSCOPT_BRKGA_H

#include <stddef.h>

#include "hscopt/decoder.h"
#include "hscopt/rng.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file hscopt_brkga.h
 * @brief Biased Random-Key Genetic Algorithm (BRKGA).
 *
 * Implementa um BRKGA para problemas de minimização utilizando representação
 * por Random Keys e avaliação via decoder.
 *
 * @note
 * - O algoritmo assume MINIMIZAÇÃO da função objetivo.
 * - Para maximização, o decoder pode retornar o valor negado.
 * - Se houver paralelismo na avaliação, o decoder deve ser thread-safe.
 */

/**
 * @struct hscopt_brkga_ctx
 * @brief Contexto opaco do BRKGA.
 *
 * O contexto mantém os hiperparâmetros e o estado do algoritmo ao longo da
 * execução. O usuário não deve acessar campos internos diretamente.
 */
typedef struct hscopt_brkga_ctx hscopt_brkga_ctx;

/**
 * @brief Cria e inicializa um contexto BRKGA.
 *
 * @param n Número de genes (random keys) por cromossomo.
 * @param p Tamanho da população.
 * @param pe Fração de indivíduos elite (0 < pe < 1).
 * @param pm Fração de mutantes (0 <= pm < 1).
 * @param rhoe Probabilidade de herança do pai elite (0 <= rhoe <= 1).
 * @param K Número de populações independentes (K >= 1).
 * @param max_threads Número máximo de threads para avaliação paralela (>= 1).
 * @param decoder Função decoder (random keys -> valor da função objetivo).
 * @param dctx Contexto do decoder (pode ser NULL).
 * @param rng Gerador de números aleatórios (obrigatório).
 *
 * @return Ponteiro para o contexto BRKGA ou NULL em caso de erro.
 *
 * @note
 * - O RNG e o decoder são referenciados (não são copiados).
 */
hscopt_brkga_ctx *hscopt_brkga_create(size_t n, size_t p, double pe, double pm,
                                      double rhoe, size_t K,
                                      unsigned max_threads,
                                      hscopt_decoder_fn decoder,
                                      hscopt_decode_ctx *dctx, hscopt_rng *rng);

/**
 * @brief Libera os recursos associados ao contexto BRKGA.
 *
 * @param ctx Contexto BRKGA (pode ser NULL).
 */
void hscopt_brkga_destroy(hscopt_brkga_ctx *ctx);

/**
 * @brief Reinicializa o BRKGA com uma nova população aleatória.
 *
 * @param ctx Contexto BRKGA.
 * @return 0 em sucesso, valor diferente de 0 em erro.
 */
int hscopt_brkga_reset(hscopt_brkga_ctx *ctx);

/**
 * @brief Evolui o BRKGA por um número de gerações.
 *
 * @param ctx Contexto BRKGA.
 * @param generations Número de gerações a evoluir (>= 1).
 * @return 0 em sucesso, valor diferente de 0 em erro.
 *
 * @note
 * - Se houver paralelismo na avaliação, o decoder deve ser thread-safe.
 */
int hscopt_brkga_evolve(hscopt_brkga_ctx *ctx, unsigned generations);

/**
 * @brief Retorna o melhor valor da função objetivo encontrado.
 *
 * O melhor valor considera todas as populações independentes.
 *
 * @param ctx Contexto BRKGA.
 * @return Melhor fitness.
 */
double hscopt_brkga_best_fitness(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o cromossomo associado ao melhor fitness.
 *
 * O melhor cromossomo considera todas as populações independentes.
 *
 * @param ctx Contexto BRKGA.
 * @return Ponteiro para o vetor de random keys do melhor indivíduo.
 *
 * @note
 * - O ponteiro retornado é válido enquanto o contexto existir e até a próxima
 *   chamada de hscopt_brkga_evolve() ou hscopt_brkga_reset().
 */
const double *hscopt_brkga_best_chromosome(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o número de genes por cromossomo.
 *
 * @param ctx Contexto BRKGA.
 * @return n (número de random keys).
 */
size_t hscopt_brkga_n_keys(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o tamanho da população.
 *
 * @param ctx Contexto BRKGA.
 * @return p (tamanho da população).
 */
size_t hscopt_brkga_p(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o número de populações independentes.
 *
 * @param ctx Contexto BRKGA.
 * @return K.
 */
size_t hscopt_brkga_k(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o número máximo de threads configurado para avaliação.
 *
 * @param ctx Contexto BRKGA.
 * @return max_threads.
 */
unsigned hscopt_brkga_max_threads(const hscopt_brkga_ctx *ctx);

/**
 * @brief Retorna o número da geração atual.
 *
 * @param ctx Contexto BRKGA.
 * @return Índice da geração (inicia em 0).
 */
unsigned hscopt_brkga_generation(const hscopt_brkga_ctx *ctx);

/**
 * @brief Realiza migração de indivíduos elite entre as populações.
 *
 * @param ctx Contexto BRKGA.
 * @param M Quantidade de elites a migrar por população (1 <= M < p).
 *
 * @return 0 em sucesso, valor diferente de 0 em erro.
 *
 * @note
 * - Requer K >= 2.
 */
int hscopt_brkga_exchange_elite(hscopt_brkga_ctx *ctx, size_t M);

#ifdef __cplusplus
}
#endif

#endif /* HSCOPT_BRKGA_H */
