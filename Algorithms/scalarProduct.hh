#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <memory>
#include <utility>

#include "Interface/abstractScalarProduct.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class. Plug your implementations in here.
   */
  class ScalarProduct
  {
  public:
    explicit ScalarProduct(std::shared_ptr<AbstractScalarProduct> implementation);

    /**
     * @brief Compute scalar product \f$(x,y)\f$, where the particular scalar product is given by the function space to which x and y belong.
     */
    auto operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractScalarProduct>()(x.impl(),y.impl()));

  private:
    std::shared_ptr<AbstractScalarProduct> impl;
  };
}

#endif // ALGORITHMS_SCALARPRODUCT_HH
