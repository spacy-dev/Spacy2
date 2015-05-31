#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <memory>
#include <utility>

#include "Interface/abstractDualPairing.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class. Plug your implementations in here.
   */
  class ScalarProduct
  {
  public:
    explicit ScalarProduct(std::shared_ptr<AbstractDualPairing> implementation);

    /**
     * @brief Compute scalar product \f$(x,y)\f$, where the particular scalar product is given by the function space to which x and y belong.
     */
    auto operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractDualPairing>()(x.impl(),y.impl()));

    /**
     * @brief Access implementation.
     */
    AbstractDualPairing const& impl() const;

  private:
    std::shared_ptr<AbstractDualPairing> impl_;
  };
}

#endif // ALGORITHMS_SCALARPRODUCT_HH
