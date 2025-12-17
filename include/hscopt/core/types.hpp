#ifndef HSCOPT_CORE_TYPES_HPP
#define HSCOPT_CORE_TYPES_HPP

#include <vector>

namespace hscopt {

/**
 * @brief Tipo usado para representar o fitness (qualidade) de uma solução
 * Por padrão é double para precisão suficiente na maioria dos problemas.
 * Se precisar mudar para float (mais rápido, menos memória), altere aqui.
 */
using FitnessType = double;

/**
 * @brief Tipo de uma chave aleatória individual
 *
 * No cada chave é um valor real no intervalo [0, 1).
 * O decoder interpreta essas chaves para construir uma solução.
 */
using KeyType = double;

/**
 * @brief Vetor de chaves aleatórias
 *
 * Cada elemento está no intervalo [0, 1).
 *
 * Exemplo para um problema com 5 variáveis de decisão:
 *   KeyVector keys = {0.234, 0.876, 0.123, 0.567, 0.901}
 */
using KeyVector = std::vector<KeyType>;

/**
 * @brief Valor representando fitness infinito (solução inválida)
 *
 * Usado para marcar soluções inviáveis em problemas de minimização.
 * Exemplo: se uma solução viola restrições, seu fitness = kInfinity.
 */
constexpr FitnessType kInfinity = 1e308;

/**
 * @brief Valor representando fitness infinito negativo
 *
 * Útil para problemas de maximização ou algoritmos que precisam
 * de um valor "pior possível".
 */
constexpr FitnessType kMinusInfinity = -1e308;

}  // namespace hscopt
#endif  // !HSCOPT_CORE_TYPES_HPP
