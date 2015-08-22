#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "vector.hh"

namespace Algorithm
{
  /// Scalar product for RealSpace.
  class RealProduct
  {
  public:
    /// Compute x*y.
    double operator()(const Vector& x, const Vector& y) const;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
