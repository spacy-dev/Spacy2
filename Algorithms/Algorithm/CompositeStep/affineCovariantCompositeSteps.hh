#ifndef ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
#define ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH

#include <memory>
#include <string>
#include <tuple>

#include "linearSolver.hh"
#include "functional.hh"
#include "vector.hh"
#include "vectorSpace.hh"
#include "../parameter.hh"
#include "Algorithm/lipschitzConstant.hh"
#include "Util/mixins.hh"

namespace Algorithm
{
  /// \cond
  namespace CompositeStep{ class CubicModel; }
  /// \endcond

  namespace CompositeStep
  {
    struct CompositeStepParameter
    {
      double etaMin = 0.5;
      double rejectionTolerance = 0.1;
      double dampingTolerance = 1e-3;
    };

    class AffineCovariantCompositeSteps :
        public CompositeStepParameter , public Mixin::RegularityTest , public Mixin::Timer<std::chrono::milliseconds> ,
        public Mixin::AdjointIndex , public Mixin::ControlIndex , public Mixin::StateIndex ,
        public Mixin::ContractionRate ,  public Mixin::Eps ,
        public Mixin::RelativeAccuracy , public Mixin::MinimalAccuracy ,
        public Mixin::Verbosity , public Mixin::MaxSteps ,
        public Mixin::IterativeRefinements
    {
      enum class StepMonitor { Rejected , Accepted };
      enum class AcceptanceTest;

    public:
      AffineCovariantCompositeSteps(const C2Functional& N, const C2Functional& L, VectorSpace& domain);

      Vector solve();

      Vector solve(const Vector& x0);

    private:
      Vector computeNormalStep(const Vector& x) const;
      Vector computeSimplifiedNormalStep(const Vector& trial) const;
      Vector computeMinimumNormCorrection(const Vector& x) const;
      Vector computeTangentialStep(double nu, const Vector& x, const Vector& dn, bool lastStepWasUndamped) const;
      Vector computeLagrangeMultiplier(const Vector& x) const;
      std::unique_ptr<GeneralLinearSolver> makeTangentialSolver(double nu, const Vector& x, bool lastStepWasUndamped) const;

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

      std::unique_ptr<C2Functional> N_, L_;
      VectorSpace& domain_;

      LipschitzConstant omegaL, omegaC;

      mutable std::unique_ptr<LinearSolver> normalSolver = nullptr;
      mutable std::unique_ptr<GeneralLinearSolver> tangentialSolver = nullptr;

      std::string spacing = "  ", spacing2 = "    ";

      StepMonitor normalStepMonitor = StepMonitor::Accepted;
      StepMonitor tangentialStepMonitor = StepMonitor::Accepted;

      double norm_dx_old = -1;
    };
  }
}

#endif // ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
