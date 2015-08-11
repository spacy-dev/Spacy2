#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "Interface/abstractVector.hh"
#include "Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  /// Scalar product for RealSpace.
  class RealProduct : public Interface::AbstractScalarProduct
  {
    /// Compute x*y.
    double operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const final override;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
