#ifndef ALGORITHM_INDUCED_SCALAR_PRODUCT_HH
#define ALGORITHM_INDUCED_SCALAR_PRODUCT_HH

#include "operator.hh"
#include "vector.hh"

namespace Algorithm
{
  /**
   * @brief Induced scalar product \f$(x,y)_M = (Mx)y\f$, where \f$M:X\rightarrow X^*\f$.
   */
  class InducedScalarProduct
  {
  public:
    /**
     * @brief Constructor.
     * @param M operator defining the scalar product.
     */
    explicit InducedScalarProduct(CallableOperator M);

    /// Compute scalar product \f$(x,y)_M\f$.
    double operator()(const Vector& x, const Vector& y) const;

  private:
    CallableOperator M_;
  };


  class PrimalInducedScalarProduct
  {
  public:
    explicit PrimalInducedScalarProduct(CallableOperator M);

    double operator()(const Vector& x, const Vector& y) const;

  private:
    CallableOperator M_;
  };
}

#endif // ALGORITHM_INDUCED_SCALAR_PRODUCT_HH
