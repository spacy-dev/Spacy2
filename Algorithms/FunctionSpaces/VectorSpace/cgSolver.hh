#ifndef ALGORITHM_VECTOR_SPACE_CGSOLVER_HH
#define ALGORITHM_VECTOR_SPACE_CGSOLVER_HH

#include <memory>

#include "../../ConjugateGradients/cg.hh"
#include "../../Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  class Operator;

  class CGSolver : public AbstractLinearSolver
  {
  public:
    CGSolver(const Operator& A);

    FunctionSpaceElement operator()(const FunctionSpaceElement &x, const FunctionSpaceElement& y) const final override;

    FunctionSpaceElement operator()(const FunctionSpaceElement& y) const final override;

  private:
    std::unique_ptr<Operator> P_;
    mutable CG cg;
  };
}

#endif // ALGORITHM_VECTOR_SPACE_CGSOLVER_HH
