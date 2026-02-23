# Build

Este projeto usa CMake modular e presets.

## Requisitos

- GCC (Linux)
- CMake >= 3.16
- Ninja (recomendado, usado nos presets)

## Build rapido (release)

```bash
cmake --preset release
cmake --build --preset release
```

## Build debug

```bash
cmake --preset debug
cmake --build --preset debug
```

## Flags (feature flags)

As opcoes abaixo podem ser ligadas/desligadas via `-D`:

- `HSCOPT_ENABLE_OPENMP` (default: ON)
- `HSCOPT_ENABLE_LTO` (default: ON)
- `HSCOPT_ENABLE_NATIVE` (default: ON)
- `HSCOPT_ENABLE_FAST_MATH` (default: OFF)
- `HSCOPT_ENABLE_WARNINGS` (default: ON)
- `HSCOPT_ENABLE_STRICT_ALIASING` (default: ON)
- `HSCOPT_ENABLE_VISIBILITY_HIDDEN` (default: ON)

Exemplo:

```bash
cmake -S . -B build -DHSCOPT_ENABLE_LTO=OFF -DHSCOPT_ENABLE_NATIVE=OFF
cmake --build build
```

## Notas

- `HSCOPT_ENABLE_FAST_MATH` pode alterar resultados numericos.
- `HSCOPT_ENABLE_NATIVE` gera binarios otimizados para a maquina atual.
