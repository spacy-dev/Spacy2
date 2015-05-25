#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <memory>
#include <utility>

#include "Interface/abstractNorm.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  /**
   * @brief Norm class. Plug your implementations in here.
   */
  class Norm
  {
  public:
    explicit Norm(std::shared_ptr<AbstractNorm> implementation);

    /**
     * @brief Compute \f$\|x\|\f$.
     */
    auto operator()(const FunctionSpaceElement& x) const -> decltype(std::declval<AbstractNorm>()(x.impl()));

  private:
    std::shared_ptr<AbstractNorm> impl_;
  };
}

#endif // ALGORITHM_NORM_HH
