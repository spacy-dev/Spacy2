#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <memory>
#include <utility>

#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class AbstractLinearSolver;
  class LinearOperator;
  class C1Operator;
  class FunctionSpaceElement;

  class LinearSolver : public Mixin::SharedImpl<AbstractLinearSolver>
  {
  public:
    LinearSolver(const LinearOperator& A);

    LinearSolver(std::shared_ptr<AbstractLinearSolver> impl);

    LinearSolver(const LinearSolver&) = delete;
    LinearSolver& operator=(const LinearSolver&) = delete;

    LinearSolver(LinearSolver&&) = default;
    LinearSolver& operator=(LinearSolver&&) = default;

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;
  };

  template <class Implementation, class... Args>
  LinearSolver makeLinearSolver(Args&&... args)
  {
    return LinearSolver( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
