#ifndef ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH

#include <memory>

#include "vector.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Interface/Operator/abstractOperator.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/Mixins/adjointIndex.hh"
#include "Util/Mixins/controlIndex.hh"
#include "Util/Mixins/stateIndex.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  class TriangularStateConstraintPreconditioner
      : public Interface::AbstractOperator, public Mixin::AdjointIndex, public Mixin::ControlIndex, public Mixin::StateIndex
  {
  public:
    TriangularStateConstraintPreconditioner(std::shared_ptr<Interface::AbstractLinearSolver> stateSolver,
                                            std::shared_ptr<Interface::AbstractLinearSolver> controlSolver,
                                            std::shared_ptr<Interface::AbstractLinearSolver> adjointSolver,
                                            std::unique_ptr<Interface::AbstractOperator>&& B,
                                            std::unique_ptr<Interface::AbstractOperator>&& BT,
                                            VectorSpace* domain,
                                            VectorSpace* range);

    Vector operator()(const Vector& x) const final override;

    Vector kernelOffset(const Vector& rhs) const;

  private:
    TriangularStateConstraintPreconditioner* cloneImpl() const;

    std::shared_ptr<Interface::AbstractLinearSolver> stateSolver_, controlSolver_, adjointSolver_;
    std::unique_ptr<Interface::AbstractOperator> B_, BT_;
  };
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_TRIANGULARSTATECONSTRAINTPRECONDITIONER_HH
