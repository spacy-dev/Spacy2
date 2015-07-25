#ifndef ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  /// Scalar product for RealSpace.
  class PrimalDualProductSpaceProduct : public Interface::AbstractScalarProduct
  {
  public:
    /// Compute x*y.
    double operator()(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
