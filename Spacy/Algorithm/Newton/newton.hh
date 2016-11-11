#pragma once

#include <Spacy/vector.hh>
#include <Spacy/Algorithm/Newton/parameter.hh>
#include <Spacy/Util/log.hh>
#include <functional>

namespace Spacy
{
  /// @cond
  class C1Operator;
  class DampingFactor;
  /// @endcond

  /** @addtogroup NewtonGroup
   * @{
   */

  namespace Newton
  {
    /**
     * @brief Generic %Newton method.
     *
     * @param F operator
     * @param x0 initial iterate
     * @param dampingStrategy damping strategy
     * @param terminationCriterion termination criterion
     * @param p parameter object holding algorithmic parameters
     *
     * @see Newton::Parameter
     */
    Vector newton(const C1Operator& F, const Vector& x0,
                  const std::function<DampingFactor(const std::function<Vector(const Vector&)>&,const Vector&,const Vector&)>& dampingStrategy,
                  const std::function<bool(DampingFactor,const Vector&,const Vector&)>& terminationCriterion,
                  const std::function<bool(const Vector&,const Vector&)>& errorEstimator,
                  const Parameter& p);

    /**
     * @brief Generic %Newton method.
     *
     * @param F operator
     * @param x0 initial iterate
     * @param p parameter object holding algorithmic parameters
     * @tparam Damping damping strategy, must provide a constructor %Damping(const C1Operator& F)
     * @tparam Terminate termination criterion, must provide a constructor Terminate(const C1Operator& F, double relativeAccuracy, bool verbose)
     *
     * @see Newton::Parameter
     */
    template <class Damping, class Terminate>
    Vector newton(const C1Operator& F, const Vector& x0, const Parameter& p, const std::function<bool(const Vector&,const Vector&)>& errorEstimator = {})
    {
      return newton(F,x0,
                    Damping(F),
                    Terminate(F,p.getRelativeAccuracy(),p.getVerbosityLevel()>1),
                    errorEstimator,p);
    }
  }

  /**
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
  Vector localNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter& p = Newton::Parameter());

  /**
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
  Vector localNewton(const C1Operator& F, const Newton::Parameter& p = Newton::Parameter());

  /**
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
  Vector covariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter& p = Newton::Parameter(), const std::function<bool(const Vector&,const Vector&)>& errorEstimator = {});

  /**
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
  Vector covariantNewton(const C1Operator& F, const Newton::Parameter& p = Newton::Parameter(), const std::function<bool(const Vector&,const Vector&)>& errorEstimator = {});

  /**
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
  Vector contravariantNewton(const C1Operator& F, const Vector& x0, const Newton::Parameter& p = Newton::Parameter());

  /**
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
  Vector contravariantNewton(const C1Operator& F, const Newton::Parameter& p = Newton::Parameter());

  /** @} */
}
