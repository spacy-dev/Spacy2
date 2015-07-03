#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <memory>
#include <utility>

#include "functionSpaceElement.hh"
#include "Interface/abstractNorm.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /**
   * @brief Norm class. Plug your implementations in here.
   */
  class Norm : public Mixin::SharedImpl<AbstractNorm>
  {
  public:
    Norm() = default;
    explicit Norm(std::shared_ptr<AbstractNorm> implementation);

    /**
     * @brief Compute \f$\|x\|\f$.
     */
    auto operator()(const FunctionSpaceElement& x) const -> decltype(std::declval<AbstractNorm>()(x.impl()));
  };
}

#endif // ALGORITHM_NORM_HH
