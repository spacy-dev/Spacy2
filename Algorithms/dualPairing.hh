#ifndef ALGORITHM_DUAL_PAIRING_HH
#define ALGORITHM_DUAL_PAIRING_HH

#include <memory>
#include <utility>

#include "Interface/abstractDualPairing.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class. Plug your implementations in here.
   */
  class DualPairing
  {
  public:
    explicit DualPairing(std::shared_ptr<AbstractDualPairing> implementation);

    /**
     * @brief Compute scalar product \f$(x,y)\f$, where the particular scalar product is given by the function space to which x and y belong.
     */
    auto operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractDualPairing>()(x.impl(),y.impl()));

  private:
    std::shared_ptr<AbstractDualPairing> impl_;
  };
}

#endif // ALGORITHM_DUAL_PAIRING_HH
