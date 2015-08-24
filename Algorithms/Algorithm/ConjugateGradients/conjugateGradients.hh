#ifndef ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH

#include <memory>
#include <string>

#include "operator.hh"
#include "vector.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/iterativeRefinements.hh"
#include "Util/Mixins/verbosity.hh"

#include "terminationCriteria.hh"

namespace Algorithm
{
  /**
   * \ingroup linalgsolution
   * \brief regularized preconditioned conjugate gradient method
   *
   * This implements a preconditioned CG iteration for an operator \f$ A: X\to x^* \f$, preconditioned by a
   * preconditioner \f$ B^{-1}: X^* \to X \f$. The default termination criterion is based on an estimate of the relative energy error.
   *
   *
   * Different implementations are available:
   *
   * - "CG": Standard preconditioned conjugate gradient method.
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
   * the used operator by \f$A\f$ and the preconditioner by \f$P\f, \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
   * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
   * regularization are neglishible.
   */
  class CGMethod :
      public Mixin::AbsoluteAccuracy ,
      public Mixin::RelativeAccuracy ,
      public Mixin::Eps ,
      public Mixin::IterativeRefinements ,
      public Mixin::Verbosity,
      public Mixin::MaxSteps
  {
    enum class Result { Converged, Failed, EncounteredNonConvexity, TruncatedAtNonConvexity };
    enum class Nonconvexity { None , Encountered };
  public:
    /**
     * \brief Set up conjugate gradient solver.
     *
     * \param A linear operator
     * \param P preconditioner
     * \param type conjugate gradient type ("CG", "RCG", "TCG" or "TRCG")
     */
    CGMethod(CallableOperator A, CallableOperator P, const std::string& type = "CG" );

    /**
     * @param x initial guess
     * @param b right hand side
     * @param tolerance tolerance of the termination criterion
     */
    Vector solve(const Vector& x, const Vector& b, double tolerance) const;

    /**
     * @param x initial guess
     * @param b right hand side
     */
    Vector solve(const Vector& x, const Vector& b) const;


    /// Set a new termination criterion which must inherit from CGTerminationCriterion.
    template <class Criterion>
    void setTerminationCriterion(const Criterion& newTerminate)
    {
      detachEps(terminate.get());
      detachAbsoluteAccuracy(terminate.get());
      detachRelativeAccuracy(terminate.get());
      detachVerbosity(terminate.get());
      detachMaxSteps(terminate.get());

      terminate.reset(std::make_unique<Criterion>(newTerminate));

      attachEps(terminate.get());
      attachAbsoluteAccuracy(terminate.get());
      attachRelativeAccuracy(terminate.get());
      attachVerbosity(terminate.get());
      attachMaxSteps(terminate.get());
    }

    /// Access to the termination criterion.
    CGTerminationCriterion& terminationCriterion() noexcept;

    /// Tells us whether non-convex directions occurred.
    bool encounteredNonConvexity() const noexcept;

    /// Energy norm of the computed solution.
    auto getEnergyNormOfSolution() const noexcept;

    void setType(const std::string& otherType);

    const CallableOperator& P() const;

    const CallableOperator& A() const;

  private:
    /// CG Implementation.
    Vector cgLoop (Vector x, Vector r) const;

    /**
     * @brief Apply preconditioner.
     */
    Vector Q(const Vector& r) const;

    /// Check step length.
    bool vanishingStep(unsigned step) const;

    /**
     * \return true if iteration should terminate. Throws if a direction of negative curvature is encountered in the standard cg implementation.
     */
    bool terminateOnNonconvexity(double qAq, double qPq, Vector& x, const Vector& q, unsigned step) const;

    /**
     * @brief Set \f$theta=0\f$.
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
    std::shared_ptr< CGTerminationCriterion > terminate = nullptr;
    mutable Result result = Result::Failed; ///< information about reason for termination
    mutable Nonconvexity nonconvexity = Nonconvexity::None;
    mutable double energyNorm2 = 0.; ///< energy norm squared

    std::string type_ = "CG";

    // parameters for regularized conjugate gradient methods
    mutable double theta = 0;
    unsigned maxIncrease = 1000, minIncrease = 2;
  };
}
#endif // ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
