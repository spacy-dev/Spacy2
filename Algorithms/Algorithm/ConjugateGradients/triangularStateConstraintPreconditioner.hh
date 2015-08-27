#ifndef ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH

#include "linearSolver.hh"
#include "operator.hh"
#include "vector.hh"
#include "Util/Mixins/adjointIndex.hh"
#include "Util/Mixins/controlIndex.hh"
#include "Util/Mixins/stateIndex.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  class TriangularStateConstraintPreconditioner
      : public OperatorBase, public Mixin::AdjointIndex, public Mixin::ControlIndex, public Mixin::StateIndex
  {
  public:
    TriangularStateConstraintPreconditioner(LinearSolver stateSolver,
                                            LinearSolver controlSolver,
                                            LinearSolver adjointSolver,
                                            CallableOperator B,
                                            CallableOperator BT,
                                            const VectorSpace& domain,
                                            const VectorSpace& range);

    /// Apply preconditioner.
    Vector operator()(const Vector& x) const;

    Vector kernelOffset(const Vector& rhs) const;

  private:
    LinearSolver stateSolver_, controlSolver_, adjointSolver_;
    CallableOperator B_, BT_;
  };
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
