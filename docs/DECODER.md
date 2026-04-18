# Decoder

Esta biblioteca assume que `hscopt_decoder_fn` e thread-safe por contrato.

## Requisitos obrigatorios

- Nao escrever em memoria compartilhada sem sincronizacao.
- Nao manter estado global mutavel sem protecao.
- Nao modificar `keys`.
- Nao modificar `ctx->inst`.
- Retornar resultado deterministico para a mesma entrada.

## Padroes recomendados

- Tratar `ctx->inst` como somente leitura.
- Guardar estado temporario por thread (TLS) ou usar buffers locais.
- Se houver estado mutavel compartilhado, usar sincronizacao explicita.
- Evitar alocacao e I/O no hot loop.

## Workspace reutilizavel (`ctx->ws`)

- `ctx->ws` pode ser usado para armazenar buffers temporarios e evitar
  alocacoes repetidas.
- Em execucao paralela, cada thread precisa de um workspace independente.
- Para isso, preencha `ctx->ws_clone` e `ctx->ws_destroy`.
- A biblioteca clona o workspace base para as demais threads durante a criacao
  do algoritmo, nunca dentro do hot loop.
- Se `ctx->ws != NULL` e `ctx->ws_clone == NULL`, a biblioteca reduz a
  avaliacao para uma thread efetiva para preservar corretude.

## Exemplo de risco

- Incrementar contador global dentro do decoder sem lock.
- Reusar buffer global unico para montar solucao.

Esses casos podem causar corrida de dados quando OpenMP estiver ativo.
