#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include "scalarProduct.hh"
#include "vector.hh"

namespace Algorithm
{
  class HilbertSpaceNorm
  {
  public:
    explicit HilbertSpaceNorm(const ScalarProduct& sp);

//    HilbertSpaceNorm(const ScalarProduct& sp);

    double scalarProduct(const Vector& x, const Vector& y) const;

    double squared(const Vector& x) const;

    double operator()(const Vector& x) const;

  private:
    ScalarProduct sp_;
  };
}

#endif // ALGORITHM_HILBERTSPACENORM_HH
