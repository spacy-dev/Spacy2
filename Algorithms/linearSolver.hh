#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <memory>

#include "operator.hh"

namespace Algorithm
{
  /// \cond
  namespace Interface { class AbstractLinearSolver; }
  class LinearOperator;
  class Vector;
  /// \endcond

  class LinearSolver : public Operator
  {
  public:
    LinearSolver() = default;

    LinearSolver(std::unique_ptr<Interface::AbstractLinearSolver>&& impl);

    Vector operator()(const Vector& x) const;

    bool encounteredNonconvexity() const;
  };
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
