#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <memory>
#include <utility>

#include "vector.hh"
#include "Interface/abstractNorm.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /**
   * @brief Norm class. Plug your implementations in here.
   */
  class Norm : public Mixin::SharedImpl<Interface::AbstractNorm>
  {
  public:
    Norm() = default;
    explicit Norm(std::shared_ptr<Interface::AbstractNorm> implementation);

    Norm(const Norm&) = delete;
    Norm& operator=(const Norm&) = delete;

    Norm(Norm&&) = default;
    Norm& operator=(Norm&&) = default;

    /**
     * @brief Compute \f$\|x\|\f$.
     */
    auto operator()(const Vector& x) const -> decltype(std::declval<Interface::AbstractNorm>()(x.impl()));
  };
}

#endif // ALGORITHM_NORM_HH
