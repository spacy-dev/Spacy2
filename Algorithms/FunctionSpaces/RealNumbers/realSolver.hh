#ifndef ALGORITHM_REALNUMBERS_REALSOLVER_HH
#define ALGORITHM_REALNUMBERS_REALSOLVER_HH

#include "../../Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  class Operator;

  class RealSolver : public AbstractLinearSolver
  {
  public:
    RealSolver(const Operator& A);

    FunctionSpaceElement operator()(const FunctionSpaceElement& y) const final override;

  private:
    const Operator& A_;
  };
}

#endif // ALGORITHM_REALNUMBERS_REALSOLVER_HH
