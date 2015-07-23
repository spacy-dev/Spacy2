#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <functional>

#include "norm.hh"

#include "newtonParameter.hh"

namespace Algorithm
{
  /// \cond
  class FunctionSpaceElement;
  class C1Operator;
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
    class NewtonMethod : public NewtonParameter
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
       * \see Newton::DampingStrategy::Undamped, Newton::DampingStrategy::AffineCovariant, Newton::DampingStrategy::AffineContravariant
       */
      template <class DampingStrategy>
      void setDampingStrategy()
      {
        dampingFactor_ = DampingStrategy(*this,F_,norm_);
      }

      /**
       * \brief Change the termination criterion.
       *
       * \see Newton::TerminationCriterion::AffineCovariant, Newton::TerminatinoCriterion::AffineContravariant
       */
      template <class TerminationCriterion>
      void setTerminationCriterion()
      {
        terminationCriterion_ = TerminationCriterion(F_,norm_,relativeAccuracy(),verbose());
      }

    private:
      const C1Operator& F_;
      std::function<double(const LinearSolver&,const FunctionSpaceElement&,const FunctionSpaceElement&)> dampingFactor_;
      std::function<bool(double,const FunctionSpaceElement&, const FunctionSpaceElement&)> terminationCriterion_;
      mutable Norm norm_;
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
