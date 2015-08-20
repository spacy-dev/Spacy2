#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include <memory>

#include "norm.hh"
#include "scalarProduct.hh"

namespace Algorithm
{
  class Vector;

  class HilbertSpaceNorm
  {
  public:
    explicit HilbertSpaceNorm(std::shared_ptr<Interface::AbstractScalarProduct> implementation);

//    HilbertSpaceNorm(const ScalarProduct& sp);

    double scalarProduct(const Vector& x, const Vector& y) const;

    double squared(const Vector& x) const;

    double operator()(const Vector& x) const;

  private:
    std::shared_ptr<Interface::AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_HILBERTSPACENORM_HH
