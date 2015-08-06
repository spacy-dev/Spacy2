#ifndef ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH

#include "abstractBanachSpace.hh"
#include "abstractScalarProduct.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractNorm;
    class AbstractFunctionSpaceElement;
    /// \endcond

    /**
     * @brief Abstract interface for hilbert spaces.
     */
    class AbstractHilbertSpace : public AbstractBanachSpace
    {
    public:
      /**
       * @brief Constructor. Sets scalar product.
       */
      AbstractHilbertSpace(std::shared_ptr<AbstractScalarProduct> sp);

      virtual ~AbstractHilbertSpace() = default;

      /// Set scalar product.
      void setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp);

      /// Get scalar product.
      std::shared_ptr<AbstractScalarProduct> scalarProduct() const;

    private:
      std::shared_ptr<AbstractScalarProduct> sp_;
    };

    /// Compute scalar product \f$(x,y)\f$.
    double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y);
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
