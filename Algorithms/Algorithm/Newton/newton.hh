#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include "Algorithms/operator.hh"
#include "Algorithms/vector.hh"
#include "Algorithms/Algorithm/Newton/dampingStrategies.hh"
#include "Algorithms/Algorithm/Newton/terminationCriteria.hh"
#include "Algorithms/Algorithm/Newton/parameter.hh"

namespace Algorithm
{
  namespace Newton
  {
    /**
     * @ingroup NewtonGroup
     * @brief Generic Newton method.
     *
     * @param F operator
     * @param x0 initial iterate
     * @param dampingStrategy damping strategy
     * @param terminationCriterion termination criterion
     * @param p parameter object holding algorithmic parameters
     *
     * - Damping strategy: Newton::DampingStrategy::None
     * - Termination criterion: Newton::TerminationCriterion::AffineCovariant
     *
     * @see Newton::Parameter, @ref Newton_DampingStrategyAnchor "DampingStrategy", @ref Newton_TerminationCriterionAnchor "TerminationCriterion"
     */
    Vector newton(const C1Operator& F, const Vector& x0,
                  const DampingStrategy& dampingStrategy,
                  const TerminationCriterion& terminationCriterion,
                  const Parameter p);

    template <class Damping, class Terminate>
    Vector newton(const C1Operator& F, const Vector& x0, const Parameter& p)
    {
      return newton(F,x0,
                    Damping(F),
                    Terminate(F,p.relativeAccuracy(),p.verbosityLevel()>1),
                    p);
    }
  }

  /**
   * @ingroup NewtonGroup
   * @brief Local Newton method.
   *
   * @param F operator
   * @param x0 initial iterate
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::None
   * - Termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector localNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Local Newton method with default initial iterate (x0=0).
   *
   * @param F operator
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::None
   * - Termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector localNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine covariant Newton method.
   *
   * @param F operator
   * @param x0 initial iterate
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::AffineCovariant
   * - Termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector covariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine covariant Newton method.
   *
   * @param F operator
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::AffineCovariant
   * - Termination criterion: Newton::TerminationCriterion::AffineCovariant
   *
   * @see Newton::Parameter
   */
  Vector covariantNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine contravariant Newton method.
   *
   * @param F operator
   * @param x0 initial iterate
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::AffineContravariant
   * - Termination criterion: Newton::TerminationCriterion::AffineContravariant
   *
   * @see Newton::Parameter
   */
  Vector contravariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter p = Newton::Parameter());

  /**
   * @ingroup NewtonGroup
   * @brief Affine contravariant Newton method.
   *
   * @param F operator
   * @param p parameter object holding algorithmic parameters
   *
   * - Damping strategy: Newton::DampingStrategy::AffineContravariant
   * - Termination criterion: Newton::TerminationCriterion::AffineContravariant
   *
   * @see Newton::Parameter
   */
  Vector contravariantNewton(const C1Operator& F, const Newton::Parameter p = Newton::Parameter());

}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
