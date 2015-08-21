#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <memory>
#include <utility>

#include "Interface/abstractScalarProduct.hh"
#include "vector.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class. Plug your implementations in here.
   */
  class ScalarProduct : public Mixin::SharedImpl<Interface::AbstractScalarProduct>
  {
  public:
    explicit ScalarProduct(std::shared_ptr<Interface::AbstractScalarProduct> implementation);

    /**
     * @brief Compute scalar product \f$(x,y)\f$.
     */
    double operator()(const Vector& x, const Vector& y) const;
  };
}

#endif // ALGORITHMS_SCALARPRODUCT_HH
