#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  namespace Interface { class AbstractFunctionSpaceElement; }

  /// Scalar product for RealSpace.
  class ProductSpaceProduct : public Interface::AbstractScalarProduct
  {
  public:
    /// Compute x*y.
    double operator()(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
