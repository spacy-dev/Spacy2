#ifndef ALGORITHM_HILBERTSPACENORM_HH
#define ALGORITHM_HILBERTSPACENORM_HH

#include <memory>

#include "Interface/abstractNorm.hh"

namespace Algorithm
{
  class AbstractScalarProduct;
  class AbstractFunctionSpaceElement;

  /**
   * @brief Norm induced by a scalar product.
   */
  class HilbertSpaceNorm : public AbstractNorm
  {
  public:
    /**
     * @brief Construct Hilbert space norm from a scalar product.
     */
    explicit HilbertSpaceNorm(std::shared_ptr<Algorithm::AbstractScalarProduct> sp);

    /// Compute \f$(x,y)\f$.
    double scalarProduct(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const;

    /// Compute \f$\|x\|\f$.
    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    /// Compute \f$\|x\|^2=(x,x)\f$.
    double squared(const AbstractFunctionSpaceElement& x) const final override;

    std::shared_ptr<AbstractScalarProduct> sp_;
  };
}

#endif // ALGORITHM_HILBERTSPACENORM_HH
