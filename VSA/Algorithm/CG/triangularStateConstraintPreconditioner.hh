#ifndef ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH

#include "VSA/linearSolver.hh"
#include "VSA/operator.hh"
#include "VSA/vector.hh"
#include "VSA/Util/Mixins/adjointIndex.hh"
#include "VSA/Util/Mixins/controlIndex.hh"
#include "VSA/Util/Mixins/stateIndex.hh"
#include "VSA/Util/Base/operatorBase.hh"

namespace VSA
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace CG
  {
    /**
     * @brief A triangular state constraint preconditioner used in @cite Lubkoll2015a.
     *
     * A preconditioner for optimal control problems of the form
     * \f[ P=\left( \begin{array}{ccc}  &  & A^{-T} \\  & M^{-1} & -B^T \\ A^{-1} & -B &  \end{array} \right)\f],
     * where \f$A\f$ is the state operator, \f$B\f$ the control operator and \f$M\f$ induces a scalar product on the control space.
     */
    class TriangularStateConstraintPreconditioner
        : public OperatorBase,
        public Mixin::AdjointIndex,
        public Mixin::ControlIndex,
        public Mixin::StateIndex
    {
    public:
      /**
       * @brief Constructor.
       * @param stateSolver solver for the evaluation of \f$A^{-1}r\f$
       * @param controlSolver solver for the evaluation of \f$M^{-1}r\f$
       * @param adjointSolver solver for the evaluation of \f$M^{-T}r\f$
       * @param B control operator
       * @param BT adjoint control operator
       * @param domain domain space
       * @param range range space
       */
      TriangularStateConstraintPreconditioner( ::VSA::LinearSolver stateSolver,
                                               ::VSA::LinearSolver controlSolver,
                                               ::VSA::LinearSolver adjointSolver,
                                               CallableOperator B,
                                               CallableOperator BT,
                                               const VectorSpace& domain,
                                               const VectorSpace& range);

      /**
       * @brief Apply preconditioner \f$P\f$.
       * @param x argument
       * @return \f$P(x)\f$
       */
      Vector operator()(const Vector& x) const;

      /**
       * @brief Compute \f$ z = \left( \begin{array}{c} z_p \\ 0 \\ 0 \end{array} \right) = \left( \begin{array}{c} A^{-1}rhs_p \\ 0 \\ 0 \end{array} \right) \f$.
       * @param rhs right hand side
       */
      Vector kernelOffset(const Vector& rhs) const;

    private:
      ::VSA::LinearSolver stateSolver_, controlSolver_, adjointSolver_;
      CallableOperator B_, BT_;
    };
  }
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
