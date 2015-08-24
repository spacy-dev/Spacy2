#ifndef ALGORITHM_HESSIAN_HH
#define ALGORITHM_HESSIAN_HH

#include <memory>

#include "functional.hh"
#include "linearSolver.hh"
#include "vector.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  /// Linear operator \f$f''(x)\f$ of a twice differentiable functional \f$f\f$.
  class Hessian : public OperatorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param A twice differentiable functional
     * @param x point of linearization
     */
    Hessian(C2Functional F, const Vector& x);

    /**
     * @brief Constructor.
     * @param A twice differentiable functional
     * @param x point of linearization
     * @param solver linear solver to (approximately) compute \f$A(x)^{-1}y\f$
     */
    Hessian(C2Functional F, const Vector &x, std::shared_ptr<LinearSolver> solver);

    /// Apply operator, i.e. compute \f$y=f''(x)dx\f$.
    Vector operator ()(const Vector& dx) const;

    /// Get linear solver (representing \f$(f''(x))^{-1}\f$).
    const LinearSolver& solver() const;

  private:
    C2Functional F_;
    Vector x_;
    std::shared_ptr<LinearSolver> solver_;
  };
}

#endif // ALGORITHM_HESSIAN_HH
