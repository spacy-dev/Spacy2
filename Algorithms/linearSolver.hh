#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <memory>
#include <utility>

#include "functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractLinearSolver;
  class C1Operator;

  class LinearSolver
  {
  public:
    LinearSolver(const C1Operator& A);

    LinearSolver(std::shared_ptr<AbstractLinearSolver> impl);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    AbstractLinearSolver& impl();

    const AbstractLinearSolver& impl() const;

  private:
    std::shared_ptr<AbstractLinearSolver> impl_;
  };

  template <class Implementation, class... Args>
  LinearSolver makeLinearSolver(Args&&... args)
  {
    return LinearSolver( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
