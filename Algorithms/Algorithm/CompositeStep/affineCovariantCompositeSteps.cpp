#include "affineCovariantCompositeSteps.hh"

#include "quadraticModel.hh"

#include "Interface/abstractLinearSolver.hh"
#include "Interface/inducedScalarProduct.hh"
#include "Util/Exceptions/regularityTestFailedException.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "Algorithm/ConjugateGradients/cgSolver.hh"
#include "Algorithm/dampingFactor.hh"

#include "c2Functional.hh"
#include "functionSpaceElement.hh"
#include "linearSolver.hh"

#include <cmath>
#include <iostream>

namespace Algorithm
{
  enum class AffineCovariantCompositeSteps::AcceptanceTest{ Passed, Failed, LeftAdmissibleDomain, TangentialStepFailed, NormalStepFailed };

  AffineCovariantCompositeSteps::AffineCovariantCompositeSteps(const C2Functional& N, const C2Functional& L)
    : N_(std::make_unique<C2Functional>(N)),
      L_(std::make_unique<C2Functional>(L))
  {}

  FunctionSpaceElement AffineCovariantCompositeSteps::solve()
  {
    return solve( N_->domain().element() );
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::solve(const FunctionSpaceElement& x0)
  {
    auto lastStepWasUndamped = false;
    auto x = x0;
    castToHilbertSpace(N_->domain()).setScalarProduct( primalInducedScalarProduct( N_->hessian(primal(x)) ) );
//    norm = HilbertSpaceNorm( primalInducedScalarProduct( N_->hessian(x0) ));

    for(unsigned step = 1; step < maxSteps(); ++step)
    {
      normalStepMonitor = tangentialStepMonitor = StepMonitor::Accepted;

      //dynamic_cast<Fenics::LagrangeFunctional&>(N_->impl()).setOrigin(x.impl());
      //norm = N_->domain().norm();

      if( verbose() ) std::cout << "\nComposite Steps: Iteration " << step << ".\n";
      if( verbose() ) std::cout << spacing << "Computing normal step." << std::endl;
      auto Dn = computeNormalStep(x);
      auto norm_Dn = norm(Dn);
      if( verbose() ) std::cout << spacing << "Computing normal damping factor" << std::endl;
      double nu = computeNormalStepDampingFactor(norm_Dn);
      if( verbose_detailed() ) std::cout << spacing2 << "|dn| = " << norm_Dn << ", nu = " << nu << std::endl;

      if( verbose() ) std::cout << spacing << "Computing lagrange multiplier." << std::endl;
      dual(x) += computeLagrangeMultiplier(x);

      if( verbose() ) std::cout << spacing << "Computing tangential step." << std::endl;
      auto Dt = computeTangentialStep(nu,x,Dn,lastStepWasUndamped);

      double tau = 0., norm_x = 0., norm_dx = 0.;
      auto ds = Dt; auto dx = Dt;

      if( verbose() ) std::cout << spacing << "Computing damping factors." << std::endl;
      std::tie(tau,dx,ds,norm_x,norm_dx) = computeCompositeStep( nu , norm_Dn , x , Dn , Dt );

      x += primal(dx);
      if( contraction() < 0.25 ) x += primal(ds);

      norm_x = norm(primal(x));

      if( nu == 1 && tau == 1 ) lastStepWasUndamped = true;
      if( convergenceTest(nu,tau,norm_x,norm_dx) ) return x;

      if( verbose() ) std::cout << spacing2 << "nu = " << nu << ", tau = " << tau << ", |dx| = " << norm_dx << std::endl;
      if( verbose() ) std::cout << spacing2 << "|x| = " << norm_x << std::endl;
      if( verbose_detailed() ) std::cout << spacing2 << "(Dn,Dt) = " << Dn*Dt/(norm_Dn*norm(Dt)) << std::endl;
    } // end iteration

    return x;
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::computeTangentialStep(double nu, const FunctionSpaceElement &x, const FunctionSpaceElement& dn, bool lastStepWasUndamped) const
  {
    if( L_==nullptr ) return FunctionSpaceElement(0*x);

    auto trcgRelativeAccuracy = minimalAccuracy();
    if( tangentialSolver != nullptr && nu == 1 && lastStepWasUndamped )
    {
      trcgRelativeAccuracy = std::max( relativeAccuracy() , std::min( minimalAccuracy() , omegaL * norm_dx_old ) );
      if( norm_dx_old > 0 && lastStepWasUndamped )
        trcgRelativeAccuracy = std::min( std::max( relativeAccuracy()/norm_dx_old , trcgRelativeAccuracy ) , minimalAccuracy() );
      if( verbose_detailed() )
      {
        std::cout << spacing2 << "relative accuracy = " << trcgRelativeAccuracy << std::endl;
        std::cout << spacing2 << "absolute step length accuracy = " << relativeAccuracy()*norm(x) << std::endl;
      }
    }
    auto trcg = std::make_unique<TRCGSolver>( L_->hessian(x) ,
                                              *normalSolver,
                                              trcgRelativeAccuracy,
                                              eps(),
                                              verbose_detailed() );
    trcg->impl().setIterativeRefinements(10);
    trcg->impl().terminationCriterion().setAbsoluteAccuracy( relativeAccuracy()*norm(x) );
    tangentialSolver = std::make_unique<LinearSolver>( std::move(trcg) );

    return primal( (*tangentialSolver)( primal(-L_->d1(x)) + primal(-nu*L_->d2(x,dn)) ) );
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::computeNormalStep(const FunctionSpaceElement &x) const
  {
    if( N_==nullptr ) return FunctionSpaceElement(0*x);

    normalSolver = std::make_unique<LinearSolver>( N_->hessian(primal(x)).solver() );
    return computeMinimumNormCorrection(x);
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::computeSimplifiedNormalStep(const FunctionSpaceElement &trial) const
  {
    if( N_==nullptr ) return FunctionSpaceElement(0*trial);
    return computeMinimumNormCorrection(trial);
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::computeMinimumNormCorrection(const FunctionSpaceElement& x) const
  {
    return primal( (*normalSolver)( dual(-L_->d1(x)) ) );
  }

  FunctionSpaceElement AffineCovariantCompositeSteps::computeLagrangeMultiplier(const FunctionSpaceElement& x) const
  {
    return dual( (*normalSolver)( primal(-L_->d1(x)) ) );
  }

  std::tuple<double,FunctionSpaceElement,FunctionSpaceElement,double,double>
  AffineCovariantCompositeSteps::computeCompositeStep(double& nu, double norm_Dn,
                                                      const FunctionSpaceElement& x, const FunctionSpaceElement& Dn, const FunctionSpaceElement& Dt)
  {
    auto norm_Dt = norm(Dt);
    if( verbose_detailed() ) std::cout << spacing2 << "|Dt| = " << norm_Dt << std::endl;
    auto cubic = CompositeStep::makeCubicModel( nu, Dn, Dt , *L_ , x , omegaL );
    auto tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubic);

    auto ds = FunctionSpaceElement(0*Dt);
    auto dx = ds;
    auto eta = 1.;
    auto norm_x = norm(x);
    auto norm_dx = 0.;
    AcceptanceTest acceptanceTest = AcceptanceTest::Failed;

    do
    {
      if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) nu *= 0.5;
      else nu = computeNormalStepDampingFactor(norm_Dn);
      if( verbose_detailed() ) std::cout << spacing2 << "nu = " << nu << std::endl;

      auto quadraticModel = CompositeStep::makeQuadraticModel(nu,Dn,Dt,*L_,x);
      auto cubicModel = CompositeStep::makeCubicModel(nu, Dn, Dt, *L_, x, omegaL);

      if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) tau *= 0.5;
      else tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubicModel);
      if( verbose_detailed() ) std::cout << spacing2 << "tau = " << tau << std::endl;
      auto q_tau = quadraticModel(tau);

      dx = primal(nu*Dn) + primal(tau*Dt);
      norm_dx = norm(primal(dx));
      if( verbose_detailed() ) std::cout << spacing2 << "|dx| = " << norm_dx << std::endl;
      auto trial = x + dx;

      if( !trial.isAdmissible() ) acceptanceTest == AcceptanceTest::LeftAdmissibleDomain;
      else
      {
        if( verbose() ) std::cout << spacing << "Computing simplified normal correction." << std::endl;
        ds = computeSimplifiedNormalStep(trial);
        ds += ( nu - 1 ) * Dn;

        updateOmegaC(norm_x, norm_dx, norm(ds));
        eta = updateOmegaL(trial + ds,q_tau,tau,norm_x,norm_dx,cubicModel);

        if( verbose_detailed() ) std::cout << spacing2 << "|ds| = " << norm(ds) << std::endl;
      }

      regularityTest(nu,tau);


      if( acceptanceTest != AcceptanceTest::LeftAdmissibleDomain ) acceptanceTest = acceptedSteps(norm_x,norm_dx,eta);

      if( acceptanceTest == AcceptanceTest::TangentialStepFailed && omegaL < (1 + 0.25 * (1 - etaMin)) * omegaL.last() )
      {
        if( verbose_detailed() ) std::cout << spacing2 << "Stagnating update of omegaL. Accepting Step." << std::endl;
        acceptanceTest = AcceptanceTest::Passed;

        if( eta < rejectionTolerance)
        {
          if( verbose_detailed() ) std::cout << spacing2 << "Ignoring tangential step." << std::endl;
          trial -= tau*Dt;
          dx -= tau*Dt;
          norm_dx = norm(dx);
        }
      }

      if( acceptanceTest == AcceptanceTest::Passed )
      {
        norm_dx_old = norm_dx;
      }

      if( verbose_detailed() )
      {
        if( acceptanceTest == AcceptanceTest::Failed ) std::cout << spacing2 << "Acceptance test failed." << std::endl;
        if( acceptanceTest == AcceptanceTest::NormalStepFailed ) std::cout << spacing2 << "Acceptance test normal step failed." << std::endl;
        if( acceptanceTest == AcceptanceTest::TangentialStepFailed ) std::cout << spacing2 << "Acceptance test tangential step failed." << std::endl;
        if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) std::cout << spacing2 << "Acceptance test left admissible domain." << std::endl;
        if( acceptanceTest == AcceptanceTest::Passed ) std::cout << spacing2 << "Acceptance test passed." << std::endl;
        if( normalStepMonitor == StepMonitor::Accepted) std::cout << spacing2 << "NormalStepMonitor::Accepted." << std::endl;
        else std::cout << spacing2 << "NormalStepMonitor::Rejected" << std::endl;
        if( tangentialStepMonitor == StepMonitor::Accepted) std::cout << spacing2 << "TangentialStepMonitor::Accepted." << std::endl;
        else std::cout << spacing2 << "TangentialStepMonitor::Rejected" << std::endl;
      }
    } // end while (damping factors)
    while( acceptanceTest != AcceptanceTest::Passed );

    return std::make_tuple(tau,dx,ds,norm_x,norm_dx);
  }

  bool AffineCovariantCompositeSteps::convergenceTest(double nu, double tau, double norm_x, double norm_dx)
  {
    if( tangentialSolver->encounteredNonconvexity() ) return false;
    if( nu < 1 || tau < 1 ) return false;

    if( norm_dx < relativeAccuracy() * norm_x || ( norm_x < eps() && norm_dx < eps() )  )
    {
      if( verbose() ) std::cout << spacing << "Terminating (convergent)." << std::endl;
      return true;
    }

    return false;
  }


  void AffineCovariantCompositeSteps::updateOmegaC(double norm_x, double norm_dx, double norm_ds)
  {
    if( norm_dx < sqrtEps() * norm_x ) return;
    setContraction( norm_ds/norm_dx );
//    if( contraction() < 0.25 && ( norm_dx < sqrtEps() * norm_x || norm_ds < eps() * norm_x ) ) return;

    if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) || omegaC < 2*contraction()/norm_dx )
      omegaC = 2*contraction()/norm_dx;

    if( verbose_detailed() ) std::cout << spacing2 << "theta = " << contraction() << ", omegaC: " << omegaC << std::endl;
  }

  double AffineCovariantCompositeSteps::updateOmegaL(const FunctionSpaceElement& soc, double q_tau,
                                                     double tau, double norm_x, double norm_dx, const CompositeStep::CubicModel& cubic)
  {
    if( tangentialSolver == nullptr ) return 1;

    double eta = 1;
    if( std::abs( cubic(tau) - cubic(0) ) > sqrtEps()*norm_x )
      eta = ( (*L_)(primal(soc)) - cubic(0) )/( cubic(tau) - cubic(0) );
    else eta = 1;

    if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) ||
        omegaL < ((*L_)(primal(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx) )
      omegaL = ((*L_)(primal(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx);

    if( verbose_detailed())
    {
      std::cout << spacing2 << "predicted decrease: " << (cubic(tau)-cubic(0)) << std::endl;
      std::cout << spacing2 << "actual decrease: " << ( (*L_)(primal(soc)) - cubic(0) ) << std::endl;
      std::cout << spacing2 << "omegaL: " << omegaL << std::endl;
      std::cout << spacing2 << "eta: " << eta << std::endl;
    }

    return eta;
  }


  double AffineCovariantCompositeSteps::computeNormalStepDampingFactor(double norm_Dn) const
  {
    DampingFactor nu = 1;
    if( norm_Dn > eps() && std::abs(norm_Dn*omegaC) > eps() ) nu = std::min(1.,desiredContraction()/(omegaC*norm_Dn));
    return nu;
  }

  double AffineCovariantCompositeSteps::computeTangentialStepDampingFactor(double norm_dn, double norm_Dt, const CompositeStep::CubicModel& cubic) const
  {
    if( norm_Dt < sqrtEps() ) return 1;

    auto maxTau = 1.;
    if( pow(relaxedDesiredContraction()/omegaC,2) - norm_dn*norm_dn > 0)
      maxTau = std::min( 1. , sqrt( pow( 2*relaxedDesiredContraction()/omegaC , 2 ) - norm_dn*norm_dn )/norm_Dt );

    DampingFactor tau = CompositeStep::findMinimizer( cubic, 0, maxTau , dampingTolerance*maxTau );

    return tau;
  }

  AffineCovariantCompositeSteps::AcceptanceTest AffineCovariantCompositeSteps::acceptedSteps(double norm_x, double norm_Dx, double eta)
  {
    if( norm_Dx < eps() * norm_x ) return AcceptanceTest::Passed;

    if( eta < etaMin )
    {
      if( verbose_detailed() ) std::cout << spacing2 << "Rejecting tangential step." << std::endl;
      tangentialStepMonitor = StepMonitor::Rejected;
      return AcceptanceTest::TangentialStepFailed;
    }

    if( !admissibleContraction() )
    {
      if( verbose_detailed() ) std::cout << spacing2 << "Rejecting normal step: " << contraction() << std::endl;
      normalStepMonitor = StepMonitor::Rejected;
      return AcceptanceTest::NormalStepFailed;
    }

    return AcceptanceTest::Passed;
  }

  void AffineCovariantCompositeSteps::regularityTest(double nu, double tau) const
  {
    if( !regularityTestPassed(nu) ) throw RegularityTestFailedException("AffineCovariantCompositeSteps::solve (nu)",nu);
    if( !regularityTestPassed(tau) ) throw RegularityTestFailedException("AffineCovariantCompositeSteps::solve (tau)",tau);
  }
}
