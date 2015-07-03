#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <memory>
#include <utility>

#include "Interface/abstractScalarProduct.hh"
#include "functionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class. Plug your implementations in here.
   */
  class ScalarProduct : public Mixin::SharedImpl<AbstractScalarProduct>
  {
  public:
    explicit ScalarProduct(std::shared_ptr<AbstractScalarProduct> implementation);

    /**
     * @brief Compute scalar product \f$(x,y)\f$.
     */
    auto operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractScalarProduct>()(x.impl(),y.impl()));
  };
}

#endif // ALGORITHMS_SCALARPRODUCT_HH
