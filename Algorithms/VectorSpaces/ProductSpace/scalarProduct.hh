#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "../../vector.hh"

namespace Algorithm
{
  namespace ProductSpace
  {
    /// Canonical scalar product on product spaces.
    class ScalarProduct
    {
    public:
      /// Compute x*y.
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
