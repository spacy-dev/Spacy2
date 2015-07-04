#ifndef ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
#define ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH

#include <memory>

#include "functionSpaceElement.hh"
#include "../../lagrangeFunctional.hh"

#include "../parameter.hh"
#include "quadraticModel.hh"
#include "Algorithm/lipschitzConstant.hh"

#include "norm.hh"

namespace Algorithm
{
  class CubicModel;

  struct CompositeStepParameter : Parameter
  {
    double thetaNormal;
    double thetaMax;
    double thetaAim;

    double rhoElbow;

    double etaMin;

    double nuMin = 1e-9;
    double tauMin = 1e-9;
    double tauMax = 1.;

    double dampingTolerance;
  };

  class AffineCovariantCompositeSteps : public CompositeStepParameter
  {
    enum class AcceptanceTest;
  public:
    AffineCovariantCompositeSteps(const LagrangeFunctional& L);

    int solve(FunctionSpaceElement& x0);

  private:
    FunctionSpaceElement computeNormalStep(const FunctionSpaceElement& x);
    FunctionSpaceElement computeSimplifiedNormalStep();
    FunctionSpaceElement computeTangentialStep();
    bool convergenceTest();

    void updateOmegaC(double norm_x, double norm_dx, double norm_ds);
    double updateOmegaL(const FunctionSpaceElement& secondOrderCorrected, double tau, double norm_x, double norm_dx, const CubicModel& cubic);

    double computeNormalStepDampingFactor(double normDn) const;
    double computeTangentialStepDampingFactor(double normdn, double normDt, const CubicModel& cubic) const;

    bool acceptedSteps(double nu, double tau, double normDx, double eta);
    bool undamped(double val) const;
    bool regularityTestFailed(double nu, double tau) const;

    const LagrangeFunctional& L_;

    LipschitzConstant omegaL, omegaC;
    double thetaC;

    std::unique_ptr<LinearSolver> normalSolver = nullptr;
    std::unique_ptr<LinearSolver> tangentialSolver = nullptr;

    Norm norm;
  };
}

#endif // ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
