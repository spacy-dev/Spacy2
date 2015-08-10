#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <memory>

#include "operator.hh"

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
      explicit NewtonMethod(const Operator& F);

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
        detachEps( dampingFactor_.get() );
        detachRegularityTest( dampingFactor_.get() );
        dampingFactor_ = std::make_shared<DampingStrategy_>(F_);
        attachEps( dampingFactor_.get() );
        attachRegularityTest( dampingFactor_.get() );
      }

      /**
       * \brief Change the termination criterion.
       *
       * \see Newton::TerminationCriterion::AffineCovariant, Newton::TerminatinoCriterion::AffineContravariant
       */
      template <class TerminationCriterion_>
      void setTerminationCriterion()
      {
        detachEps( terminationCriterion_.get() );
        detachVerbosity( terminationCriterion_.get() );
        detachRelativeAccuracy( terminationCriterion_.get() );
        terminationCriterion_ = std::make_shared<TerminationCriterion_>(F_,relativeAccuracy(),verbose());
        attachEps( terminationCriterion_.get() );
        attachVerbosity( terminationCriterion_.get() );
        attachRelativeAccuracy( terminationCriterion_.get() );
      }

    private:
      Operator F_;
      std::shared_ptr<DampingStrategy::Base> dampingFactor_;
      std::shared_ptr<TerminationCriterion::Base> terminationCriterion_;
    };
  }

  /**
   * @brief Get local Newton Method.
   *
   * damping strategy: Newton::DampingStrategy::Undamped
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Newton::NewtonMethod localNewton(const Operator& F);

  /**
   * @brief Get affine covariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineCovariant
   * termination criterion: Newton::TerminationCriterion::AffineCovariant
   */
  Newton::NewtonMethod covariantNewton(const Operator& F);

  /**
   * @brief Get affine contravariant Newton method.
   *
   * damping strategy: Newton::DampingStrategy::AffineContravariant
   * termination criterion: Newton::TerminationCriterion::AffineContravariant
   */
  Newton::NewtonMethod contravariantNewton(const Operator& F);
}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
