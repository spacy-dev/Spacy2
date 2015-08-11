#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  namespace Interface { class AbstractVector; }

  /// Scalar product for RealSpace.
  class ProductSpaceProduct : public Interface::AbstractScalarProduct
  {
  public:
    /// Compute x*y.
    double operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const final override;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
