#pragma once

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>

namespace Spacy
{
  /// Induced scalar product \f$(x,y)_M = (Mx)y\f$, where \f$M:X\rightarrow X^*\f$.
  class AdaptiveInducedScalarProduct
  {
  public:
    /**
     * @brief Constructor.
     * @param M operator defining the scalar product.
     */
    explicit AdaptiveInducedScalarProduct(std::function<LinearOperator()> operatorCreator);

    /// Compute scalar product \f$(x,y)_M\f$.
    Real operator()(const Vector& x, const Vector& y) const;

  private:
//    LinearOperator M_;
    std::function<LinearOperator()> operatorCreator_;
  };

  /// Induced scalar product \f$(x,y)_M = (Mx)y\f$, where \f$M:X\rightarrow X^*\f$.
  class InducedScalarProduct
  {
  public:
    /**
     * @brief Constructor.
     * @param M operator defining the scalar product.
     */
    explicit InducedScalarProduct(LinearOperator M);

    /// Compute scalar product \f$(x,y)_M\f$.
    Real operator()(const Vector& x, const Vector& y) const;

  private:
    LinearOperator M_;
  };

  /**
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
     * @param M linear operator defining the scalar product.
     */
    explicit PrimalInducedScalarProduct(LinearOperator M);

    /// @brief Compute scalar product \f$(x,y)_M\f$.
    Real operator()(const Vector& x, const Vector& y) const;

  private:
    LinearOperator M_;
  };
}
