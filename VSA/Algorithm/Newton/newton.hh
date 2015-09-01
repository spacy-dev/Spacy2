#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include "VSA/operator.hh"
#include "VSA/vector.hh"
#include "VSA/Algorithm/Newton/dampingStrategies.hh"
#include "VSA/Algorithm/Newton/terminationCriteria.hh"
#include "VSA/Algorithm/Newton/parameter.hh"

namespace VSA
{
  namespace Newton
  {
    /**
     * @ingroup NewtonGroup
     * @brief Generic %Newton method.
     *
     * @param F operator
     * @param x0 initial iterate
     * @param dampingStrategy damping strategy
     * @param terminationCriterion termination criterion
     * @param p parameter object holding algorithmic parameters
     *
     * @see Newton::Parameter, @ref Newton_DampingStrategyAnchor "DampingStrategy", @ref Newton_TerminationCriterionAnchor "TerminationCriterion"
     */
    Vector newton(const C1Operator& F, const Vector& x0,
                  const DampingStrategy& dampingStrategy,
                  const TerminationCriterion& terminationCriterion,
                  const Parameter p);

    /**
     * @ingroup NewtonGroup
     * @brief Generic %Newton method.
     *
     * @param F operator
     * @param x0 initial iterate
     * @param p parameter object holding algorithmic parameters
     * @tparam Damping damping strategy, must provide a constructor %Damping(const C1Operator& F)
     * @tparam Terminate termination criterion, must provide a constructor Terminate(const C1Operator& F, double relativeAccuracy, bool verbose)
     *
     * @see Newton::Parameter, @ref Newton_DampingStrategyAnchor "DampingStrategy", @ref Newton_TerminationCriterionAnchor "TerminationCriterion"
     */
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
   * @brief Local %Newton method.
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
   * @brief Local %Newton method with default initial iterate (x0=0).
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
   * @brief Affine covariant %Newton method.
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
   * @brief Affine covariant %Newton method.
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
   * @brief Affine contravariant %Newton method.
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
   * @brief Affine contravariant %Newton method.
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
