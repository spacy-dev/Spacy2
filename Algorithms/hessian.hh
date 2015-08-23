#ifndef ALGORITHM_HESSIAN_HH
#define ALGORITHM_HESSIAN_HH

#include "functional.hh"
#include "linearSolver.hh"
#include "vector.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  class Hessian : public OperatorBase
  {
  public:
    Hessian(C2Functional F, const Vector& x);

    Vector operator ()(const Vector& dx) const;

    LinearSolver solver() const;

  private:
    C2Functional F_;
    Vector x_;
  };
}

#endif // ALGORITHM_HESSIAN_HH
