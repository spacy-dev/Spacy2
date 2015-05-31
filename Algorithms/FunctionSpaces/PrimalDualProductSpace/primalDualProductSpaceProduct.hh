#ifndef ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractDualPairing.hh"

namespace Algorithm
{
  /// Scalar product for RealSpace.
  class PrimalDualProductSpaceProduct : public AbstractDualPairing
  {
  public:
    /// Compute x*y.
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // ALGORITHM_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
