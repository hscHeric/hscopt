#ifndef HSCOPT_RNG_H
#define HSCOPT_RNG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file hscopt_rng.h
 * @brief RNG rápido baseado em xoshiro256**.
 *
 * Implementa um gerador pseudo-aleatório de alta performance (xoshiro256**),
 *
 * @note
 * - O estado do xoshiro256** possui 256 bits (4x uint64_t).
 * - O estado não pode ser todo zero.
 * - Não é thread-safe se a mesma instância for compartilhada entre threads.
 */

/**
 * @struct hscopt_rng
 * @brief Estado do RNG xoshiro256**.
 *
 * Cada instância mantém seu próprio estado.
 */
typedef struct hscopt_rng {
  /**
   * Estado interno do gerador (256 bits).
   * Não deve ser todo zero.
   */
  uint64_t s[4];
} hscopt_rng;

/**
 * @brief Inicializa o RNG com uma seed 64-bit.
 *
 * A seed é expandida internamente para 256 bits usando SplitMix64.
 *
 * @param rng Ponteiro para o gerador.
 * @param seed Valor inicial. Pode ser 0.
 */
void hscopt_rng_seed(hscopt_rng *rng, uint64_t seed);

/**
 * @brief Gera o próximo número pseudo-aleatório (64-bit).
 *
 * @param rng Ponteiro para o gerador.
 * @return Próximo valor pseudo-aleatório.
 */
uint64_t hscopt_rng_next_u64(hscopt_rng *rng);

/**
 * @brief Avança o estado em 2^128 passos (subsequência não sobreposta).
 *
 * Útil para paralelismo: gera uma nova subsequência independente.
 *
 * @param rng Ponteiro para o gerador.
 */
void hscopt_rng_jump(hscopt_rng *rng);

/**
 * @brief Avança o estado em 2^192 passos (subsequência não sobreposta).
 *
 * Útil para paralelismo distribuído.
 *
 * @param rng Ponteiro para o gerador.
 */
void hscopt_rng_long_jump(hscopt_rng *rng);

/**
 * @brief Gera um double uniforme em [0,1).
 *
 * @param rng Ponteiro para o gerador.
 * @return Valor em [0,1).
 */
static inline double hscopt_rng_next_u01(hscopt_rng *rng) {
  return (hscopt_rng_next_u64(rng) >> 11) *
         (1.0 / 9007199254740992.0); /* 2^53 */
}

#ifdef __cplusplus
}
#endif

#endif /* HSCOPT_RNG_H */
