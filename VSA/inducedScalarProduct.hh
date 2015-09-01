#ifndef ALGORITHM_INDUCED_SCALAR_PRODUCT_HH
#define ALGORITHM_INDUCED_SCALAR_PRODUCT_HH

#include "VSA/operator.hh"
#include "VSA/vector.hh"

namespace VSA
{
  /**
   * @ingroup VSpaceGroup
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

    /**
     * @brief Compute scalar product.
     * @param x vector
     * @param y vector
     * @return \f$(x,y)_M\f$.
     */
    double operator()(const Vector& x, const Vector& y) const;

  private:
    CallableOperator M_;
  };

  /**
   * @ingroup VSpaceGroup
   * @brief Induced scalar product for the primal variables (i.e. for constrained optimization problems).
   *
   * Let
   * \f$ M = \left( \begin{array}{ccc} A & B^T \\ B &  \end{array} \right)\f$.
   * Then the primal induced scalar product is
   * \f[ (z_1,z_2)_M = (Ax_1)(x_2) + (p_1,p_2)_Q  \f]
   * for \f$z_i = (x_i,p_i)\in X\times Q,\ i=1,2\f$.
   */
  class PrimalInducedScalarProduct
  {
  public:
    /**
     * @brief Constructor.
     * @param M operator defining the scalar product.
     */
    explicit PrimalInducedScalarProduct(CallableOperator M);

    /**
     * @brief Compute scalar product.
     * @param x vector
     * @param y vector
     * @return \f$(x,y)_M\f$.
     */
    double operator()(const Vector& x, const Vector& y) const;

  private:
    CallableOperator M_;
  };
}

#endif // ALGORITHM_INDUCED_SCALAR_PRODUCT_HH
