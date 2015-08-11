#ifndef ALGORITHM_INTERFACE_HILBERTSPACENORM_HH
#define ALGORITHM_INTERFACE_HILBERTSPACENORM_HH

#include <memory>

#include "Interface/abstractNorm.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractScalarProduct;
    class AbstractVector;

    /**
   * @brief Norm induced by a scalar product.
   */
    class HilbertSpaceNorm : public AbstractNorm, public Mixin::SharedImpl<AbstractScalarProduct>
    {
    public:
      /**
     * @brief Construct Hilbert space norm from a scalar product.
     */
      explicit HilbertSpaceNorm(std::shared_ptr<AbstractScalarProduct> sp);

      HilbertSpaceNorm(const HilbertSpaceNorm&) = delete;
      HilbertSpaceNorm& operator=(const HilbertSpaceNorm&) = delete;

      HilbertSpaceNorm(HilbertSpaceNorm&&) = default;
      HilbertSpaceNorm& operator=(HilbertSpaceNorm&&) = default;

      /// Compute \f$(x,y)\f$.
      double scalarProduct(const AbstractVector& x, const AbstractVector& y) const;

      /// Compute \f$\|x\|\f$.
      double operator()(const AbstractVector& x) const final override;

      /// Compute \f$\|x\|^2=(x,x)\f$.
      double squared(const AbstractVector& x) const final override;
    };
  }
}

#endif // ALGORITHM_INTERFACE_HILBERTSPACENORM_HH
