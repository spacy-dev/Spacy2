#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "../../Interface/abstractDualPairing.hh"

namespace Algorithm
{
  class AbstractFunctionSpaceElement;

  /// Scalar product for RealSpace.
  class ProductSpaceProduct : public AbstractDualPairing
  {
  public:
    /// Compute x*y.
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
