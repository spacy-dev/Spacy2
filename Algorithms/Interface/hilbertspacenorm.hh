#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include <memory>

#include "abstractNorm.hh"

namespace Algorithm
{
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  class HilbertSpaceNorm : public AbstractNorm
  {
  public:
    explicit HilbertSpaceNorm(std::shared_ptr<AbstractScalarProduct> sp);

    double scalarProduct(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const;

    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    double squared(const AbstractFunctionSpaceElement& x) const final override;

    std::shared_ptr<AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_HILBERTSPACENORM_HH
