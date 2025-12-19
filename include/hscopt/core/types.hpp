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
}  // namespace hscopt

#endif  // !HSCOPT_CORE_TYPES_HPP
