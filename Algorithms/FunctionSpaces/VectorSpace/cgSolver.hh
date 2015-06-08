#ifndef ALGORITHM_VECTOR_SPACE_CGSOLVER_HH
#define ALGORITHM_VECTOR_SPACE_CGSOLVER_HH

#include <memory>

#include "Algorithm/ConjugateGradients/cg.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  class Operator;
  class LinearOperator;
  class AbstractOperator;

  class CGSolver : public AbstractLinearSolver
  {
  public:
    CGSolver(const Operator& A);
    CGSolver(const LinearOperator& A);

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& y) const final override;

  private:
    Operator P_;
    mutable CG cg;
  };
}

#endif // ALGORITHM_VECTOR_SPACE_CGSOLVER_HH
