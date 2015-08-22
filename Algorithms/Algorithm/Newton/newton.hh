#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include "vector.hh"
#include "dampingStrategies.hh"
#include "terminationCriteria.hh"
#include "parameter.hh"

namespace Algorithm
{
  /// \cond
  class LinearSolver;
  /// \endcond

  namespace Newton
  {    
    Vector newton(const Operator& F, const Vector& x0,
                  const DampingStrategy::Base& dampingFactor,
                  const TerminationCriterion::Base& terminationCriterion,
                  const Parameter p);

    template <class Damping, class Terminate>
    Vector newton(const Operator& F, const Vector& x0, const Parameter& p)
    {
      return newton(F,x0,
                    Damping(F),
                    Terminate(F,p.relativeAccuracy(),p.verbose_detailed()),
                    p);
    }
  }

  /**
   * @brief Local Newton Method.
   *
   * damping strategy: Newton::DampingStrategy::Undamped
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Vector localNewton(const Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  Vector localNewton(const Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @brief Affine covariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineCovariant
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Vector covariantNewton(const Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  Vector covariantNewton(const Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @brief Affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Vector contravariantNewton(const Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @brief Affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Vector contravariantNewton(const Operator& F, const Newton::Parameter p = Newton::Parameter());

}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
