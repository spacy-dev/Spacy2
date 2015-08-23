#ifndef ALGORITHM_HESSIAN_HH
#define ALGORITHM_HESSIAN_HH

#include <memory>

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

    Hessian(C2Functional F, const Vector &x, std::shared_ptr<LinearSolver> solver);

    Vector operator ()(const Vector& dx) const;

    LinearSolver solver() const;

  private:
    C2Functional F_;
    Vector x_;
    std::shared_ptr<LinearSolver> solver_;
  };
}

#endif // ALGORITHM_HESSIAN_HH
