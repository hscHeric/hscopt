# Contribuindo

Obrigado por contribuir. Este guia descreve o fluxo basico.

## Padrao de commits

Use um formato inspirado em Conventional Commits:

tipo(escopo): mensagem curta

- `tipo`: `feat`, `fix`, `docs`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`
- `escopo` (opcional): area afetada (ex.: `hho`, `rvns`, `rng`, `docs`, `build`).
- mensagem no imperativo, minuscula, sem ponto final.

Exemplos:
- `docs(hho): documentar API publica`
- `fix(rng): corrigir overflow no salto`
- `feat(rvns): adicionar estrategia de shaking`

## Codigo e estilo

- Preferir C simples e claro.
- Evitar alocacoes no hot loop.
- Manter funcoes pequenas e bem nomeadas.
- Documentar APIs publicas com Doxygen.

## Testes

- Compile com `cmake --preset release` e `cmake --build --preset release`.
- Se alterar API, atualize `README.md` e `BUILD.md`.

## Issues e PRs

- Descreva o problema e a motivacao.
- Inclua passos para reproduzir quando aplicavel.
- Inclua resultados de build/testes.
