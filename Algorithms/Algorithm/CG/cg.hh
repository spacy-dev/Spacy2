#ifndef ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH

#include <memory>
#include <string>

#include "Algorithms/operator.hh"
#include "Algorithms/vector.hh"
#include "Algorithms/Util/Mixins/eps.hh"
#include "Algorithms/Util/Mixins/iterativeRefinements.hh"
#include "Algorithms/Util/Mixins/verbosity.hh"

#include "terminationCriteria.hh"

namespace Algorithm
{
  namespace CG
  {
    /**
     * \ingroup CGGroup
     * \brief Conjugate gradient method.
     *
     * This implements a preconditioned CG iteration for an operator \f$ A: X\to x^* \f$, preconditioned by a
     * preconditioner \f$ B^{-1}: X^* \to X \f$. The default termination criterion is based on an estimate of the relative
     * energy error (see StrakosTichyEnergyError).
     *
     *
     * Different implementations are available:
     *
     * - "CG": Standard preconditioned conjugate gradient method (see @cite Hestenes1952).
     *
     * - "TCG": Truncated preconditioned conjugate gradient method for nonconvex problems. Stops iteration if a direction of negative curvature is
     * encountered.
     *
     * - "RCG": Regularized preconditioned conjugate gradient method for nonconvex problems. Denote the used operator by \f$A\f$ and the preconditioner by \f$P\f$.
     * Then if a direction of negative curvature is encountered \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
     * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
     * regularization are neglishible.
     *
     * - "TRCG": Truncated regularized preconditioned conjugate gradient method for nonconvex problems, mixing the truncated with the regularized conjugate gradient method. If a direction
     * of negative curvature is encounted and the termination criterion indicates sufficient decrease in the used norm the iteration is stopped. Else, denoting
     * the used operator by \f$A\f$ and the preconditioner by \f$P\f$, \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
     * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
     * regularization are neglishible (see @cite Lubkoll2015a).
     */
    class Solver :
        public Mixin::AbsoluteAccuracy ,
        public Mixin::RelativeAccuracy ,
        public Mixin::Eps ,
        public Mixin::IterativeRefinements ,
        public Mixin::Verbosity,
        public Mixin::MaxSteps
    {
      enum class Result { Converged, Failed, EncounteredNonConvexity, TruncatedAtNonConvexity };
      enum class DefiniteNess { PositiveDefinite , Indefinite };
    public:
      /**
       * \brief Set up conjugate gradient solver.
       *
       * \param A linear operator
       * \param P preconditioner
       * \param type conjugate gradient type ("CG", "RCG", "TCG" or "TRCG")
       */
      Solver(CallableOperator A, CallableOperator P, const std::string& type = "CG" );

      /**
       * @param x initial guess
       * @param b right hand side
       */
      Vector solve(const Vector& x, const Vector& b) const;


      /**
       * @brief Set a new termination criterion which must satisfy the @ref CG_TerminationCriterionConceptAnchor "TerminationCriterionConcept".
       * @param newTerminate new termination criterion
       */
      template <class Criterion>
      void setTerminationCriterion(Criterion newTerminate)
      {
  //      detachEps(terminate);
  //      detachAbsoluteAccuracy(terminate);
  //      detachRelativeAccuracy(terminate);

        terminate = std::move(newTerminate);

  //      attachEps(terminate);
  //      attachAbsoluteAccuracy(terminate);
  //      attachRelativeAccuracy(terminate);
      }

//      /// Access to the termination criterion.
//      CG::TerminationCriterion& terminationCriterion() noexcept;

      /**
       * @brief After solving an equation reports if the underlying operator is indefinite.
       * @return true: if \f$A\f$ is indefinite, else false
       */
      bool indefiniteOperator() const noexcept;

//      /// Energy norm of the computed solution.
//      auto getEnergyNormOfSolution() const noexcept;

      /**
       * @brief Change conjugate gradient implementation.
       * @param otherType conjugate gradient type ("CG", "RCG", "TCG" or "TRCG")
       */
      void setType(const std::string& otherType);

      /**
       * @brief Access preconditioner.
       * @return preconditioner \f$P\f$
       */
      const CallableOperator& P() const;

      /**
       * @brief Access operator.
       * @return operator \f$A\f$
       */
      const CallableOperator& A() const;

    private:
      /// CG Implementation.
      Vector cgLoop (Vector x, Vector r) const;

      /**
       * @brief Apply preconditioner.
       */
      Vector Q(const Vector& r) const;

      /// Check if step length is below maximal attainable accuracy.
      bool vanishingStep(unsigned step) const;

      /**
       * @brief Behaviour if a direction of negative curvature is encountered.
       *
       * Throws if a direction of negative curvature is encountered in the standard cg implementation.
       *
       * @return true if iteration should terminate.
       */
      bool terminateOnNonconvexity(double qAq, double qPq, Vector& x, const Vector& q, unsigned step) const;

      /**
       * @brief Set \f$\theta=0\f$.
       */
      void initializeRegularization() const noexcept;

      /**
       * @brief Replace \f$qAq\f$ with \f$ qAq + \theta qPq\f$.
       */
      void regularize(double& qAq, double qPq) const noexcept;

      /**
       * @brief Update regularization parameter \f$\theta\f$ in regularized conjugate gradient methods.
       */
      void updateRegularization(double qAq, double qPq) const;

      /**
       * @brief Replace \f$r\f$ with \f$ r - \alpha\theta\Pq \f$ in regularized conjugate gradient methods.
       */
      void adjustRegularizedResidual(double alpha, const Vector& Pq, Vector& r) const;


      CallableOperator A_, P_;
      mutable CG::TerminationCriterion terminate;
      mutable Result result = Result::Failed; ///< information about reason for termination
      mutable DefiniteNess definiteness_ = DefiniteNess::PositiveDefinite;
      mutable double energyNorm2 = 0.; ///< energy norm squared

      std::string type_ = "CG";

      // parameters for regularized conjugate gradient methods
      mutable double theta = 0;
      unsigned maxIncrease = 1000, minIncrease = 2;
    };
  }
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
