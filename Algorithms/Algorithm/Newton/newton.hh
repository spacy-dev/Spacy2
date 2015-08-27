#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include "operator.hh"
#include "vector.hh"
#include "dampingStrategies.hh"
#include "terminationCriteria.hh"
#include "parameter.hh"

namespace Algorithm
{
  namespace Newton
  {

    Vector newton(const C1Operator& F, const Vector& x0,
                  const DampingStrategy::Base& dampingFactor,
                  const TerminationCriterion::Base& terminationCriterion,
                  const Parameter p);

    template <class Damping, class Terminate>
    Vector newton(const C1Operator& F, const Vector& x0, const Parameter& p)
    {
      return newton(F,x0,
                    Damping(F),
                    Terminate(F,p.relativeAccuracy(),p.verbose_detailed()),
                    p);
    }
  }

  /**
   * @ingroup NewtonGroup
   * @brief Local Newton Method.
   *
   * @param F operator
   * @param x0 initial iterate
   * @param p parameter object holding algorithmic parameters
   *
   * damping strategy: Newton::DampingStrategy::Undamped
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector localNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Local Newton Method with default initial iterate (x0=0).
   *
   * @param F operator
   * @param p parameter object holding algorithmic parameters
   *
   * damping strategy: Newton::DampingStrategy::Undamped
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector localNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine covariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineCovariant
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Vector covariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  Vector covariantNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Vector contravariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Vector contravariantNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
