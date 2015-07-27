#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <memory>

#include "operator.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  class LinearOperator;
  class C1Operator;
  class FunctionSpaceElement;
  /// \endcond

  class LinearSolver : public Operator
//      public Mixin::SharedImpl<Interface::AbstractLinearSolver>
  {
  public:
    LinearSolver() = default;
    //zLinearSolver(const LinearOperator& A);

    LinearSolver(std::unique_ptr<Interface::AbstractLinearSolver>&& impl);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    bool encounteredNonconvexity() const;
  };
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
