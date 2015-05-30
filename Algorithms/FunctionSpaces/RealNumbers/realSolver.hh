#ifndef ALGORITHM_REALNUMBERS_REALSOLVER_HH
#define ALGORITHM_REALNUMBERS_REALSOLVER_HH

#include "../../Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  class C0Operator;

  class RealSolver : public AbstractLinearSolver
  {
  public:
    RealSolver(const C0Operator& A);

    FunctionSpaceElement operator()(const FunctionSpaceElement& y) const final override;

  private:
    const C0Operator& A_;
  };
}

#endif // ALGORITHM_REALNUMBERS_REALSOLVER_HH
