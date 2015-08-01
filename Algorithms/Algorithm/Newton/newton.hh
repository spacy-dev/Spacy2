#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <memory>

#include "c1Operator.hh"

#include "dampingStrategies.hh"
#include "terminationCriteria.hh"
#include "Util/mixins.hh"

namespace Algorithm
{
  /// \cond
  class FunctionSpaceElement;
  class LinearSolver;
  /// \endcond

  namespace Newton
  {
    /**
     * \ingroup NewtonGroup
     * \brief Newton's method.
     *
     * Both the damping strategy and termination criterion can be adjusted.
     * \see Newton::DampingStrategy::Undamped, Newton::DampingStrategy::AffineCovariant, Newton::DampingStrategy::AffineContravariant
     * \see Newton::TerminationCriterion::AffineCovariant, Newton::TerminatinoCriterion::AffineContravariant
     */
    class NewtonMethod :
        public Mixin::Eps, public Mixin::RelativeAccuracy, public Mixin::Verbosity,
        public Mixin::MaxSteps , public Mixin::RegularityTest , public Mixin::Timer
    {
    public:
      /**
       * @brief Set up Newton's method to solve \f$F(x)=0\f$.
       */
      explicit NewtonMethod(const C1Operator& F);

      /**
       * \brief Apply Newton's method to solve \f$F(x)=0\f$ starting at the initial guess \f$x0=0\f$.
       * \return \f$x\f$ such that \f$ F(x) = 0 \f$ (up to the desired accuracy).
       */
      FunctionSpaceElement solve() const;

      /**
       * \brief Apply Newton's method to solve \f$F(x)=0\f$ starting at the initial guess \f$x0\f$.
       * \param \f$x0\f$ initial guess
       * \return \f$x\f$ such that \f$ F(x) = 0 \f$ (up to the desired accuracy).
       */
      FunctionSpaceElement solve(const FunctionSpaceElement& x0) const;

      /**
       * \brief Change the damping strategy.
       *
       * \see Newton::DampingStrategy::None, Newton::DampingStrategy::AffineCovariant, Newton::DampingStrategy::AffineContravariant
       */
      template <class DampingStrategy_>
      void setDampingStrategy()
      {
        dampingFactor_ = std::make_unique<DampingStrategy_>(F_);
        connectEps( *dampingFactor_ );
        connectRegularityTest( *dampingFactor_ );
      }

      /**
       * \brief Change the termination criterion.
       *
       * \see Newton::TerminationCriterion::AffineCovariant, Newton::TerminatinoCriterion::AffineContravariant
       */
      template <class TerminationCriterion_>
      void setTerminationCriterion()
      {
        terminationCriterion_ = std::make_unique<TerminationCriterion_>(F_,relativeAccuracy(),verbose());
        connectVerbosity( *terminationCriterion_ );
        connectRelativeAccuracy( *terminationCriterion_ );
      }

    private:
      C1Operator F_;
      std::unique_ptr<DampingStrategy::Base> dampingFactor_;
      std::unique_ptr<TerminationCriterion::Base> terminationCriterion_;
    };
  }

  /**
   * @brief Get local Newton Method.
   *
   * damping strategy: Newton::DampingStrategy::Undamped
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Newton::NewtonMethod localNewton(const C1Operator& F);

  /**
   * @brief Get affine covariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineCovariant
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Newton::NewtonMethod covariantNewton(const C1Operator& F);

  /**
   * @brief Get affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Newton::NewtonMethod contravariantNewton(const C1Operator& F);
}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
