#include <stdint.h>
#include <stdio.h>

#include "hscopt/rng.h"

/*
 * Exemplo de uso do gerador de números aleatórios  da hscopt.
 *
 * Demonstra:
 * - inicialização com seed
 * - geração de inteiros aleatórios
 * - geração de doubles em [0,1)
 *
 * Compile (exemplo):
 *  gcc rng_example.c -I../include ../src/rng.c -o example_rng
 */

int main(void) {
  hscopt_rng rng;

  // Inicializando rng com seed fixa para reprodutabilidade
  hscopt_rng_seed(&rng, 123456789ULL);

  printf("Inteiros aleatórios (uint64_t): \n");
  for (int i = 0; i < 5; ++i) {
    uint64_t x = hscopt_rng_next_u64(&rng);
    printf("  %llu\n", (unsigned long long)x);
  }

  printf("\nDoubles aleatórios em [0,1):\n");
  for (int i = 0; i < 5; ++i) {
    double d = hscopt_rng_next_u01(&rng);
    printf("  %.10f\n", d);
  }
}
