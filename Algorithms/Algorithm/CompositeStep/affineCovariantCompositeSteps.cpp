//#include "affineCovariantCompositeSteps.hh"

//#include "quadraticModel.hh"

//namespace Algorithm
//{

//  enum class AffineCovariantCompositeSteps::AcceptanceTest{ Passed, Failed, LeftAdmissibleDomain, TangentialStepFailed, NormalStepFailed };

//  AffineCovariantCompositeSteps::AffineCovariantCompositeSteps()
//  {
//  }

//  int AffineCovariantCompositeSteps::solve(FunctionSpaceElement& x0)
//  {
//    auto x = x0;
//    double norm_x = norm(x);

//    for(unsigned step = 1; step < param.maxSteps; ++step)
//    {
//      auto dx = x;

//      auto dn = computeNormalStep(x);
//      auto norm_Dn = norm(dn);
//      double nu = computeNormalStepDampingFactor(norm_Dn);

//      auto dt = computeTangentialStep(x,nu,dn);
//      auto norm_Dt = norm(dt);

//      auto cubic = makeCubicModel( nu, dn, dt, sp, L );
//      double tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubic);

//      while( acceptedSteps(nu,dn,tau,dt) != AcceptanceTest::Passed )
//      {
//        double eta = 1;
//        nu = computeNormalStepDampingFactor(norm_Dn);
//        tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubic);
//        dx = nu*dn + tau*dt;
//        auto trial = x + dx;

//        if( regularityTestFailed(nu,tau) ) return -1;
//      }


//      x += dx + ds;
//      norm_x = norm(x);
//    }
//  }


//  void AffineCovariantCompositeSteps::updateOmegaC(double norm_x, double norm_dx, double norm_ds)
//  {
//    thetaC = norm_ds/norm_dx;
//    if( C.theta < 0.25 && ( norm_dx < param.sqrt_eps * norm_x || norm_ds < p.eps * norm_x ) ) return;
//    omegaC = 2*thetaC/norm_dx;
//  }

//  double AffineCovariantCompositeSteps::updateOmegaL(const FunctionSpaceElement& secondOrderCorrected, double tau, double norm_x, double norm_dx, const CubicModel& cubic)
//  {
//    double eta = 1;

//    if( std::fabs(cubic(tau) - cubic(0)) > param.sqrt_eps * norm_x )
//    {
//      auto deltaf = L(primal(secondOrderCorrected)) - cubic(0);
//      eta = deltaf/( cubic(tau) - cubic(0) );
//      omegaL = deltaf*6/(norm_dx*norm_dx*norm_dx);
//    }

//    return eta;
//  }


//  double AffineCovariantCompositeSteps::computeNormalStepDampingFactor(double norm_Dn) const
//  {
//    double nu = 1;
//    if( norm_Dn > param.eps && std::fabs(norm_Dn*omegaC) > param.eps ) nu = std::min(1.,param.thetaNormal/(omegaC*norm_Dn));
//    return nu;
//  }

//  double AffineCovariantCompositeSteps::computeTangentialStepDampingFactor(double norm_dn, double norm_Dt, const CubicModel& cubic) const
//  {
//    if( normDt < p.sqrt_eps ) return 1;

//    double maxTau = sqrt( pow( 2*param.thetaAim/omegaC , 2 ) - norm_dn*norm_dn );
//    if( maxTau > 0 && norm_Dt > 0 ) maxTau = std::min(1.,maxTau/norm_Dt);

//    return findMinimizer( cubic, 0., maxTau , param.sqrt_eps*std::min(1.,maxTau) );
//  }

//  bool AffineCovariantCompositeSteps::acceptedSteps(double nu, double tau, double normDx, double eta)
//  {
//    if( normDx < param.eps ) return AcceptanceTest::Passed;
//    if( undamped(nu) && undamped(tau) && normDx < param.sqrt_eps ) return AcceptanceTest::Passed;
//    if( eta < param.etaMin ) return AcceptanceTest::TangentialStepFailed;
//    if( thetaC > param.thetaMax ) return AcceptanceTest::NormalStepFailed;

//    return AcceptanceTest::Failed;
//  }

//  bool AffineCovariantCompositeSteps::regularityTestFailed(double nu, double tau) const
//  {
//    return nu > param.nuMin && tau > param.tauMin;
//  }

//  bool AffineCovariantCompositeSteps::undamped(double val) const
//  {
//    return std::fabs(val-1.) < AbstractBanachSpacedampingTolerance;
//  }
//}
