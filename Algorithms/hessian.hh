#ifndef ALGORITHM_HESSIAN_HH
#define ALGORITHM_HESSIAN_HH

#include <memory>

#include "Algorithms/functional.hh"
#include "Algorithms/linearSolver.hh"
#include "Algorithms/vector.hh"
#include "Algorithms/Util/Base/operatorBase.hh"

namespace Algorithm
{
  /**
   * @ingroup VHatGroup
   * @brief Linear operator \f$f''(x)\f$ for a twice differentiable functional \f$f\f$.
   */
  class Hessian : public OperatorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param F twice differentiable functional
     * @param x point of linearization
     */
    Hessian(C2Functional F, Vector x);

    /**
     * @brief Constructor.
     * @param F twice differentiable functional
     * @param x point of linearization
     * @param solver linear solver to (approximately) compute \f$A(x)^{-1}y\f$
     */
    Hessian(C2Functional F, Vector x, LinearSolver solver);

    /// Apply operator, i.e. compute \f$y=f''(x)dx\f$.
    Vector operator ()(const Vector& dx) const;

    /// Get linear solver (representing \f$(f''(x))^{-1}\f$).
    const LinearSolver& solver() const;

  private:
    C2Functional F_;
    Vector x_;
    LinearSolver solver_ = {};
  };
}

#endif // ALGORITHM_HESSIAN_HH
