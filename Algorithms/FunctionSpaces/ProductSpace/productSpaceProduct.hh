#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "vector.hh"

namespace Algorithm
{
  /// Canonical scalar product on product spaces.
  class ProductSpaceProduct
  {
  public:
    /// Compute x*y.
    double operator()(const Vector& x, const Vector& y) const;
  };
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
