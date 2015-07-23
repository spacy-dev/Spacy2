#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <memory>

#include "operator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// \cond
  namespace Interface { class AbstractLinearSolver; }
  class LinearOperator;
  class C1Operator;
  class FunctionSpaceElement;
  /// \endcond

  class LinearSolver :
      public Mixin::SharedImpl<Interface::AbstractLinearSolver>
  {
  public:
    LinearSolver() = default;
    LinearSolver(const LinearOperator& A);

    LinearSolver(std::shared_ptr<Interface::AbstractLinearSolver> impl);

    LinearSolver(const LinearSolver&) = delete;
    LinearSolver& operator=(const LinearSolver&) = delete;

    LinearSolver(LinearSolver&&) = default;
    LinearSolver& operator=(LinearSolver&&) = default;

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    bool encounteredNonconvexity() const;
  };
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
