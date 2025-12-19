#ifndef HSCOPT_CORE_DECODER_HPP
#define HSCOPT_CORE_DECODER_HPP

#include "types.hpp"

namespace hscopt {
template <typename TSolution>
class IDecoder {
  virtual ~IDecoder() = default;

  // Transforma vetor de chaves [0,1] em solução
  virtual TSolution decode(const KeyVector& keys) const = 0;

  // Calcula fitness da solução
  virtual FitnessType evaluate(const TSolution& solution) const = 0;

  // Dimensão do vetor de chaves
  [[nodiscard]] virtual size_t dimension() const = 0;

  // avaliação direta
  [[nodiscard]] virtual double decode_and_evaluate(
      const std::vector<double>& keys) const {
    return evaluate(decode(keys));
  }

  // Opcional: informações para otimizações
  [[nodiscard]] virtual bool is_thread_safe() const { return true; }
};
}  // namespace hscopt

#endif  // !HSCOPT_CORE_DECODER_HPP
