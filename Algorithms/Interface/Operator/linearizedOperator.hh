#ifndef ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
#define ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH

#include <memory>

#include "abstractC1Operator.hh"
#include "abstractLinearOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractFunctionSpaceElement;
    /// \endcond

    /// Linearized operator \f$A'(x)\f$ of a differentiable operator \f$A\f$.
    class LinearizedOperator : public AbstractLinearOperator
    {
    public:
      /**
       * @brief Constructor.
       * @param A (possibly nonlinear) differentiable operator.
       * @param x point of linearization.
       */
      LinearizedOperator(std::unique_ptr<AbstractC1Operator>&& A, const AbstractFunctionSpaceElement& x);

      /// Apply operator, i.e. compute \f$y=A'(x)dx\f$.
      std::unique_ptr<AbstractFunctionSpaceElement> operator ()(const AbstractFunctionSpaceElement& dx) const final override;

      /// Get linear solver (representing \f$(A'(x))^{-1}\f$).
      std::unique_ptr<AbstractLinearSolver> solver() const final override;

    private:
      LinearizedOperator* cloneImpl() const;

      std::unique_ptr<AbstractC1Operator> A_;
      std::unique_ptr<AbstractFunctionSpaceElement> x_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
