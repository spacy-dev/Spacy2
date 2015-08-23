#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "../../vector.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Real
  {
    /// Scalar product for RealSpace.
    class ScalarProduct
    {
    public:
      /// Compute x*y.
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH