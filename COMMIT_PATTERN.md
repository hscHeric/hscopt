# Padrao de commits

Este repositorio usa um padrao inspirado em Conventional Commits.

## Formato

tipo(escopo): mensagem curta

- `tipo`: categoria da mudanca.
- `escopo` (opcional): area afetada (ex.: `hho`, `rvns`, `rng`, `docs`, `build`).
- `mensagem curta`: verbo no imperativo, minusculo, sem ponto final.

## Tipos recomendados

- `feat`: nova funcionalidade.
- `fix`: correcao de bug.
- `docs`: documentacao.
- `refactor`: refatoracao sem mudanca de comportamento.
- `perf`: melhoria de desempenho.
- `test`: adicao/ajuste de testes.
- `build`: sistema de build/dependencias.
- `ci`: pipelines/CI.
- `chore`: tarefas gerais (lint, limpeza, atualizacoes menores).

## Quebra de compatibilidade

Use `!` apos o tipo/escopo e descreva no corpo:

tipo(escopo)!: mensagem curta

BREAKING CHANGE: descreva o impacto e o caminho de migracao.

## Exemplos

- `docs(hho): documentar API publica`
- `fix(rng): corrigir overflow no salto`
- `feat(rvns): adicionar estrategia de shaking`
- `refactor(core): simplificar validacoes`
