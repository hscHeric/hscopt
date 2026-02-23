# hscopt

Biblioteca C de metaheuristicas para otimizacao combinatoria baseada em random keys.

## Visao geral

- Representacao por random keys no hipercubo [0,1)
- Algoritmos: Harris Hawks Optimization (HHO) e RVNS
- RNG xoshiro256** com funcoes de salto
- API simples e focada em desempenho

## Estrutura

- `include/hscopt/` headers publicos
- `src/` implementacoes
- `examples/` exemplos de uso
- `docs/` guias de build e contribuicao

## Build

Veja `docs/BUILD.md`.

## Uso rapido

Header principal:

```c
#include "hscopt/hscopt.h"
```

Fluxo tipico:

- gerar random keys em [0,1)
- avaliar via `hscopt_decoder_fn`
- iterar com `hscopt_hho_iterate` ou `hscopt_rvns_iterate`

Exemplos disponiveis:

- `examples/rng_example.c`
- `examples/decoder_example.c`
- `examples/hho_example.c`

## Notas

- As chaves sao tratadas como random keys em [0,1).
- O clamp do dominio e aplicado internamente nas iteracoes do HHO e RVNS.
- A avaliacao e feita via `hscopt_decoder_fn`.

## Contribuindo

Veja `docs/CONTRIBUTING.md`.
