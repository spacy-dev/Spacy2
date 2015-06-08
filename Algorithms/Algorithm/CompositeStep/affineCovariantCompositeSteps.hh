//#ifndef ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
//#define ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH

//#include "../functionSpaceElement.hh"
//#include "../../lagrangeFunctional.hh"

//#include "../parameter.hh"
//#include "quadraticModel.hh"

//namespace Algorithm
//{
//  class CubicModel;
//  class Norm;
//  class ScalarProduct;

//  struct CompositeStepParameter : Parameter
//  {
//    double thetaNormal;
//    double thetaMax;
//    double thetaAim;

//    double rhoElbow;

//    double etaMin;

//    double nuMin = 1e-9;
//    double tauMin = 1e-9;
//    double tauMax = 1.;

//    double dampingTolerance;

//    double eps = 1e-12; // maximal attainable tolerance
//    double sqrt_eps = 1e-6;
//    double cbrt_eps = 1e-4;
//  };

//  class AffineCovariantCompositeSteps
//  {
//    enum class AcceptanceTest;
//  public:
//    AffineCovariantCompositeSteps();

//    int solve(FunctionSpaceElement& x0);

//  private:
//    FunctionSpaceElement computeNormalStep();
//    FunctionSpaceElement computeSimplifiedNormalStep();
//    FunctionSpaceElement computeTangentialStep();
//    bool convergenceTest();

//    void updateOmegaC(double norm_x, double norm_dx, double norm_ds);
//    double updateOmegaL(const FunctionSpaceElement& secondOrderCorrected, double tau, double norm_x, double norm_dx, const CubicModel& cubic);

//    double computeNormalStepDampingFactor(double normDn) const;
//    double computeTangentialStepDampingFactor(double normdn, double normDt, const CubicModel& cubic) const;

//    bool acceptedSteps(double nu, double tau, double normDx, double eta);
//    bool undamped(double val) const;
//    bool regularityTestFailed(double nu, double tau) const;

//    LagrangeFunctional L;
//    CompositeStepParameter param;

//    LipschitzConstant omegaL, omegaC;
//    double thetaC;

//    LinearSolver normalSolver;
//    LinearSolver tangentialSolver;

//    Norm norm;
//    ScalarProduct sp;
//  };
//}

//#endif // ALGORITHM_AFFINECOVARIANTCOMPOSITESTEPS_HH
