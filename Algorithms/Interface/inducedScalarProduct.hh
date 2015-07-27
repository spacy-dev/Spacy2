#ifndef ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
#define ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH

#include "abstractScalarProduct.hh"
#include "Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  class Operator;
  class ScalarProduct;
  /// \endcond

  namespace Interface
  {
    /**
     * @brief Induced scalar product \f$(x,y)_M = (Mx)y\f$, where \f$M:X\rightarrow X^*\f$.
     */
    class InducedScalarProduct : public AbstractScalarProduct, public Mixin::UniqueImpl<AbstractOperator>
    {
    public:
      /**
       * @brief Constructor.
       * @param M operator defining the scalar product.
       */
      explicit InducedScalarProduct(const AbstractOperator& M);

      /// Compute scalar product \f$(x,y)_M\f$.
      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
    };


    class PrimalInducedScalarProduct : public AbstractScalarProduct, public Mixin::UniqueImpl<AbstractOperator>
    {
    public:
      explicit PrimalInducedScalarProduct(const AbstractOperator& M);

      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
    };
  }

  /**
   * @brief Create induced scalar product.
   * @param A Operator defining scalar product.
   * @return ScalarProduct(std::make_shared<Interface::InducedScalarProduct>( A.impl() ) );
   */
  ScalarProduct inducedScalarProduct(const Operator& A);

  /**
   * @brief Create primal induced scalar product.
   * @param A Operator defining scalar product.
   * @return ScalarProduct(std::make_shared<Interface::PrimalInducedScalarProduct>( A.impl() ) );
   */
  ScalarProduct primalInducedScalarProduct(const Operator& A);
}

#endif // ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
