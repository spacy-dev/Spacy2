#ifndef ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
#define ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH

#include <memory>
#include <string>
#include <tuple>

#include "../parameter.hh"
#include "quadraticModel.hh"
#include "Algorithm/lipschitzConstant.hh"
#include "Algorithm/ConjugateGradients/cg.hh"
#include "Util/mixins.hh"
#include "hilbertSpaceNorm.hh"

namespace Algorithm
{
  /// \cond
  class C2Functional;
  class CubicModel;
  class FunctionSpaceElement;
  class LinearSolver;
  /// \endcond

  struct CompositeStepParameter : Parameter , Mixin::ContractionRate
  {
    double etaMin = 0.5;
    double rejectionTolerance = 0.1;

    double dampingTolerance = 1e-5;

  };

  class AffineCovariantCompositeSteps :
      public CompositeStepParameter , public Mixin::RegularityTest , public Mixin::MinimalAccuracy ,
      public Mixin::AdjointIndex , public Mixin::ControlIndex , public Mixin::StateIndex , public Mixin::Timer
  {
    enum class StepMonitor { Rejected , Accepted };
    enum class AcceptanceTest;
  public:
    AffineCovariantCompositeSteps(const C2Functional& N, const C2Functional& L);

    FunctionSpaceElement solve();

    FunctionSpaceElement solve(const FunctionSpaceElement& x0);

  private:
    FunctionSpaceElement computeNormalStep(const FunctionSpaceElement& x) const;
    FunctionSpaceElement computeSimplifiedNormalStep(const FunctionSpaceElement& trial) const;
    FunctionSpaceElement computeMinimumNormCorrection(const FunctionSpaceElement& x) const;
    FunctionSpaceElement computeTangentialStep(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement& dn, bool lastStepWasUndamped) const;
    FunctionSpaceElement computeLagrangeMultiplier(const FunctionSpaceElement& x) const;

    bool convergenceTest(double nu, double tau, double norm_x, double norm_dx);

    std::tuple<double, FunctionSpaceElement, FunctionSpaceElement, double, double> computeCompositeStep(double& nu, double norm_Dn,
                const FunctionSpaceElement& x, const FunctionSpaceElement& Dn, const FunctionSpaceElement& Dt);

    void updateOmegaC(double norm_x, double norm_dx, double norm_ds);
    double updateOmegaL(const FunctionSpaceElement& soc, double q_tau,
                        double tau, double norm_x, double norm_dx, const CompositeStep::CubicModel& cubic);

    double computeNormalStepDampingFactor(double normDn) const;
    double computeTangentialStepDampingFactor(double normdn, double normDt, const CompositeStep::CubicModel& cubic) const;

    void regularityTest(double nu, double tau) const;
    AcceptanceTest acceptedSteps(double norm_x, double normDx, double eta);

    std::unique_ptr<C2Functional> N_, L_;

    LipschitzConstant omegaL, omegaC;

    mutable std::unique_ptr<LinearSolver> normalSolver = nullptr, tangentialSolver = nullptr;

    std::string spacing = "  ", spacing2 = "    ";

    StepMonitor normalStepMonitor = StepMonitor::Accepted;
    StepMonitor tangentialStepMonitor = StepMonitor::Accepted;

    double norm_dx_old = -1;
  };
}

#endif // ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
