#ifndef ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
#define ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH

#include "vector.hh"
#include "Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  class Operator;
  /// \endcond

//  namespace Interface
//  {
    /**
     * @brief Induced scalar product \f$(x,y)_M = (Mx)y\f$, where \f$M:X\rightarrow X^*\f$.
     */
    class InducedScalarProduct : public Mixin::UniqueImpl<Interface::AbstractOperator>
    {
    public:
      /**
       * @brief Constructor.
       * @param M operator defining the scalar product.
       */
      explicit InducedScalarProduct(const Interface::AbstractOperator& M);

      /// Compute scalar product \f$(x,y)_M\f$.
      double operator()(const Vector& x, const Vector& y) const;
    };


    class PrimalInducedScalarProduct : public Mixin::UniqueImpl<Interface::AbstractOperator>
    {
    public:
      explicit PrimalInducedScalarProduct(const Interface::AbstractOperator& M);

      double operator()(const Vector& x, const Vector& y) const;
    };
//  }

  /**
   * @brief Create induced scalar product.
   * @param A Operator defining scalar product.
   * @return ScalarProduct(std::make_shared<Interface::InducedScalarProduct>( A.impl() ) );
   */
  InducedScalarProduct inducedScalarProduct(const Operator& A);

  /**
   * @brief Create primal induced scalar product.
   * @param A Operator defining scalar product.
   * @return ScalarProduct(std::make_shared<Interface::PrimalInducedScalarProduct>( A.impl() ) );
   */
  PrimalInducedScalarProduct primalInducedScalarProduct(const Operator& A);
}

#endif // ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
