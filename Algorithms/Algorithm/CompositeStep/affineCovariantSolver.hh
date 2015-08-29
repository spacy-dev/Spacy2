#ifndef ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
#define ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH

#include <memory>
#include <string>
#include <tuple>

#include "Algorithms/linearSolver.hh"
#include "Algorithms/functional.hh"
#include "Algorithms/vector.hh"
#include "Algorithms/vectorSpace.hh"
#include "Algorithms/Algorithm/lipschitzConstant.hh"
#include "Algorithms/Util/mixins.hh"

namespace Algorithm
{
  /// \cond
  namespace CompositeStep{ class CubicModel; }
  /// \endcond

  namespace CompositeStep
  {
    /**
     * @brief The affine covariant step method described in @cite Lubkoll2015, @cite Lubkoll2015a for the solution of equality constraint optimization problems.
     *
     * An affine covariant composite step method for the solution of problems of the form
     * \f[\min f(x)\quad \text{s.t.}\quad c(x)=0\f], based on the corresponding Lagrange functional
     * \f[L(x,p) = f(x)+pc(x)\f].
     */
    class AffineCovariantSolver :
        public Mixin::RegularityTest ,
        public Mixin::Timer<std::chrono::milliseconds> ,
        public Mixin::AdjointIndex ,
        public Mixin::ControlIndex ,
        public Mixin::StateIndex ,
        public Mixin::ContractionRate ,
        public Mixin::DecreaseCondition ,
        public Mixin::Eps ,
        public Mixin::MaxSteps ,
        public Mixin::DampingAccuracy ,
        public Mixin::MinimalAccuracy ,
        public Mixin::RelativeAccuracy ,
        public Mixin::Verbosity ,
        public Mixin::IterativeRefinements
    {
      enum class StepMonitor { Rejected , Accepted };
      enum class AcceptanceTest;

    public:
      /**
       * @brief Constructor.
       * @param N Lagrange functional for the problem \f[\min \|\delta x_k\| \quad \text{s.t.} c'(x_k)\delta x_k + c(x_k)=0\f]
       * @param L Lagrange functional
       * @param domain domain space \f$X=\{Y,U,P\}\f$
       */
      AffineCovariantSolver(const C2Functional& N, const C2Functional& L, VectorSpace& domain);

      /// Compute solution.
      Vector solve();

      /**
       * @brief Compute solution.
       * @param x0 initial iterate
       */
      Vector solve(const Vector& x0);

    private:
      Vector computeNormalStep(const Vector& x) const;
      Vector computeSimplifiedNormalStep(const Vector& trial) const;
      Vector computeMinimumNormCorrection(const Vector& x) const;
      Vector computeTangentialStep(double nu, const Vector& x, const Vector& dn, bool lastStepWasUndamped) const;
      Vector computeLagrangeMultiplier(const Vector& x) const;
      std::unique_ptr<IndefiniteLinearSolver> makeTangentialSolver(double nu, const Vector& x, bool lastStepWasUndamped) const;

      bool convergenceTest(double nu, double tau, double norm_x, double norm_dx);

      std::tuple<double, Vector, Vector, double, double> computeCompositeStep(double& nu, double norm_Dn,
                  const Vector& x, const Vector& Dn, const Vector& Dt);

      void updateOmegaC(double norm_x, double norm_dx, double norm_ds);
      double updateOmegaL(const Vector& soc, double q_tau,
                          double tau, double norm_x, double norm_dx, const CompositeStep::CubicModel& cubic);

      double computeNormalStepDampingFactor(double normDn) const;
      double computeTangentialStepDampingFactor(double normdn, double normDt, const CompositeStep::CubicModel& cubic) const;

      void regularityTest(double nu, double tau) const;
      AcceptanceTest acceptedSteps(double norm_x, double normDx, double eta);

      std::unique_ptr<C2Functional> N_ = nullptr, L_ = nullptr;
      VectorSpace& domain_;

      LipschitzConstant omegaL = {1e-6}, omegaC = {1e-6};

      mutable std::unique_ptr<LinearSolver> normalSolver = nullptr;
      mutable std::unique_ptr<IndefiniteLinearSolver> tangentialSolver = nullptr;

      std::string spacing = "  ", spacing2 = "    ";

      StepMonitor normalStepMonitor = StepMonitor::Accepted;
      StepMonitor tangentialStepMonitor = StepMonitor::Accepted;

      double norm_dx_old = -1;
    };
  }
}

#endif // ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
