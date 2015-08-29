#include "affineCovariantCompositeSteps.hh"

#include "Algorithms/Algorithm/dampingFactor.hh"
#include "Algorithms/Algorithm/CG/linearSolver.hh"
#include "Algorithms/Algorithm/CG/triangularStateConstraintPreconditioner.hh"
#include "Algorithms/Algorithm/CompositeStep/quadraticModel.hh"

#include "Algorithms/inducedScalarProduct.hh"
#include "Algorithms/Util/Exceptions/regularityTestFailedException.hh"

#include <cmath>
#include <iostream>

namespace Algorithm
{
  namespace CompositeStep
  {
    enum class AffineCovariantCompositeSteps::AcceptanceTest{ Passed, Failed, LeftAdmissibleDomain, TangentialStepFailed, NormalStepFailed };

    AffineCovariantCompositeSteps::AffineCovariantCompositeSteps(const C2Functional& N, const C2Functional& L, VectorSpace& domain)
      : N_(std::make_unique<C2Functional>(N)),
        L_(std::make_unique<C2Functional>(L)),
        domain_(domain)
    {}

    Vector AffineCovariantCompositeSteps::solve()
    {
      return solve( domain_.vector() );
    }

    Vector AffineCovariantCompositeSteps::solve(const Vector& x0)
    {
      auto lastStepWasUndamped = false;
      auto x = x0;

      for(unsigned step = 1; step < maxSteps(); ++step)
      {
        normalStepMonitor = tangentialStepMonitor = StepMonitor::Accepted;

        domain_.setScalarProduct( PrimalInducedScalarProduct( N_->hessian(primal(x)) ) );

        if( verbose() ) std::cout << "\nComposite Steps: Iteration " << step << ".\n";
        if( verbose() ) std::cout << spacing << "Computing normal step." << std::endl;
        auto Dn = computeNormalStep(x);
        auto norm_Dn = norm(Dn);
        if( verbose() )
        {
          std::cout << spacing << "Normal step length: " << norm_Dn << std::endl;
          std::cout << spacing << "Computing normal damping factor" << std::endl;
        }
        double nu = computeNormalStepDampingFactor(norm_Dn);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "|dn| = " << norm_Dn << ", nu = " << nu << std::endl;

        if( verbose() ) std::cout << spacing << "Computing lagrange multiplier." << std::endl;
        dual(x) += computeLagrangeMultiplier(x);

        if( verbose() ) std::cout << spacing << "Computing tangential step." << std::endl;
        auto Dt = computeTangentialStep(nu,x,Dn,lastStepWasUndamped);

        double tau = 0., norm_x = 0., norm_dx = 0.;
        auto ds = Dt; auto dx = Dt;

        if( verbose() )
        {
          std::cout << spacing << "Tangential step length: " << norm(Dt) << std::endl;
          std::cout << spacing << "Computing damping factors." << std::endl;
        }
        std::tie(tau,dx,ds,norm_x,norm_dx) = computeCompositeStep( nu , norm_Dn , x , Dn , Dt );

        x += primal(dx);
        if( contraction() < 0.25 ) x += primal(ds);

        norm_x = norm(primal(x));

        if( nu == 1 && tau == 1 ) lastStepWasUndamped = true;
        if( convergenceTest(nu,tau,norm_x,norm_dx) ) return x;

        if( verbose() ) std::cout << spacing2 << "nu = " << nu << ", tau = " << tau << ", |dx| = " << norm_dx << std::endl;
        if( verbose() ) std::cout << spacing2 << "|x| = " << norm_x << std::endl;
        if( verbosityLevel() > 1) std::cout << spacing2 << "(Dn,Dt) = " << Dn*Dt/(norm_Dn*norm(Dt)) << std::endl;
      } // end iteration

      return x;
    }

    Vector AffineCovariantCompositeSteps::computeTangentialStep(double nu, const Vector &x, const Vector& dn, bool lastStepWasUndamped) const
    {
      if( L_==nullptr ) return Vector(0*x);

      tangentialSolver = makeTangentialSolver(nu,x,lastStepWasUndamped);

      return primal( (*tangentialSolver)( primal(-L_->d1(x)) + primal(-nu*L_->d2(x,dn)) ) );
    }

    std::unique_ptr<IndefiniteLinearSolver> AffineCovariantCompositeSteps::makeTangentialSolver(double nu, const Vector &x, bool lastStepWasUndamped) const
    {
      auto trcgRelativeAccuracy = minimalAccuracy();
      if( tangentialSolver != nullptr && nu == 1 && lastStepWasUndamped )
      {
        trcgRelativeAccuracy = std::max( relativeAccuracy() , std::min( minimalAccuracy() , omegaL * norm_dx_old ) );
        if( norm_dx_old > 0 && lastStepWasUndamped )
          trcgRelativeAccuracy = std::min( std::max( relativeAccuracy()/norm_dx_old , trcgRelativeAccuracy ) , minimalAccuracy() );
        if( verbosityLevel() > 1 )
        {
          std::cout << spacing2 << "relative accuracy = " << trcgRelativeAccuracy << std::endl;
          std::cout << spacing2 << "absolute step length accuracy = " << relativeAccuracy()*norm(x) << std::endl;
        }
      }

      auto setParams = [this,&x](auto& solver)
      {
        solver.setIterativeRefinements(iterativeRefinements());
        solver.setVerbosityLevel( verbosityLevel() );
        if( norm(primal(x)) > 0)
          solver.setAbsoluteAccuracy( relativeAccuracy()*norm(primal(x)) );
        else
          solver.setAbsoluteAccuracy( eps() );
        solver.setMaxSteps(maxSteps());
      };

  //    std::unique_ptr<CGSolver> trcg = nullptr;

      if( is<CG::LinearSolver>(*normalSolver) )
      {
        const auto& cgSolver = cast_ref<CG::LinearSolver>(*normalSolver);
        if( is<CG::TriangularStateConstraintPreconditioner>(cgSolver.P()))
        {
          auto trcg =  makeTRCGSolver( L_->hessian(x) , cgSolver.P() ,
                                       trcgRelativeAccuracy , eps() , verbose() );
          setParams(trcg);
          return std::make_unique<IndefiniteLinearSolver>(trcg);
        }
      }

  //    if( trcg == nullptr )
       auto trcg = makeTRCGSolver( L_->hessian(x) , *normalSolver ,
                                   trcgRelativeAccuracy , eps(), verbose() );
  //    trcg.setIterativeRefinements(iterativeRefinements());
  //    trcg.setDetailedVerbosity(verbose_detailed());
  //    if( norm(primal(x)) > 0)
  //      trcg.setAbsoluteAccuracy( relativeAccuracy()*norm(primal(x)) );
  //    else
  //      trcg.setAbsoluteAccuracy( eps() );
  //    trcg.setMaxSteps(maxSteps());
       setParams(trcg);
      return std::make_unique<IndefiniteLinearSolver>(trcg);
      //return std::move(trcg);
  //    return std::unique_ptr<IndefiniteLinearSolver>( trcg.release() );
    }

    Vector AffineCovariantCompositeSteps::computeNormalStep(const Vector &x) const
    {
      if( N_==nullptr ) return Vector(0*x);

      normalSolver = std::make_unique<LinearSolver>( N_->hessian(primal(x)).solver() );
      return computeMinimumNormCorrection(x);
    }

    Vector AffineCovariantCompositeSteps::computeSimplifiedNormalStep(const Vector &trial) const
    {
      if( N_==nullptr ) return Vector(0*trial);
      return computeMinimumNormCorrection(trial);
    }

    Vector AffineCovariantCompositeSteps::computeMinimumNormCorrection(const Vector& x) const
    {
      auto rhs = dual(-L_->d1(x));
      Vector dn0 = 0*x;
      if( is<CG::LinearSolver>(*normalSolver) )
      {
        auto& cgSolver = cast_ref<CG::LinearSolver>(*normalSolver);
        cgSolver.setEps(eps());
        cgSolver.setRelativeAccuracy(eps());
        cgSolver.setVerbosity(verbose());
        cgSolver.setVerbosityLevel(verbosityLevel());
        cgSolver.setIterativeRefinements(iterativeRefinements());
        cgSolver.setMaxSteps(maxSteps());
        if( is<CG::TriangularStateConstraintPreconditioner>(cgSolver.P()))
        {
          const auto& P = cast_ref<CG::TriangularStateConstraintPreconditioner>(cgSolver.P());
          dn0 = P.kernelOffset(rhs);
          rhs -= cgSolver.A()( dn0 );
        }
      }
      return dn0 + primal( (*normalSolver)( rhs ) );
    }

    Vector AffineCovariantCompositeSteps::computeLagrangeMultiplier(const Vector& x) const
    {
      if( N_ == nullptr || L_ == nullptr ) return Vector(0*x);
      return dual( (*normalSolver)( primal(-L_->d1(x)) ) );
    }

    std::tuple<double,Vector,Vector,double,double>
    AffineCovariantCompositeSteps::computeCompositeStep(double& nu, double norm_Dn,
                                                        const Vector& x, const Vector& Dn, const Vector& Dt)
    {
      auto norm_Dt = norm(Dt);
      if( verbosityLevel() > 1 ) std::cout << spacing2 << "|Dt| = " << norm_Dt << std::endl;
      auto cubic = CompositeStep::makeCubicModel( nu, Dn, Dt , *L_ , x , omegaL );
      auto tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubic);

      auto ds = Vector(0*Dt);
      auto dx = ds;
      auto eta = 1.;
      auto norm_x = norm(x);
      auto norm_dx = 0.;
      AcceptanceTest acceptanceTest = AcceptanceTest::Failed;

      do
      {
        if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) nu *= 0.5;
        else nu = computeNormalStepDampingFactor(norm_Dn);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "nu = " << nu << std::endl;

        auto quadraticModel = CompositeStep::makeQuadraticModel(nu,Dn,Dt,*L_,x);
        auto cubicModel = CompositeStep::makeCubicModel(nu, Dn, Dt, *L_, x, omegaL);

        if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) tau *= 0.5;
        else tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubicModel);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "tau = " << tau << std::endl;
        auto q_tau = quadraticModel(tau);

        dx = primal(nu*Dn) + primal(tau*Dt);
        norm_dx = norm(primal(dx));
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "|dx| = " << norm_dx << std::endl;
        auto trial = x + dx;

        if( !domain_.isAdmissible(trial) ) acceptanceTest = AcceptanceTest::LeftAdmissibleDomain;
        else
        {
          if( verbose() ) std::cout << spacing << "Computing simplified normal correction." << std::endl;
          ds = computeSimplifiedNormalStep(trial);
          ds += ( nu - 1 ) * Dn;

          updateOmegaC(norm_x, norm_dx, norm(ds));
          eta = updateOmegaL(trial + ds,q_tau,tau,norm_x,norm_dx,cubicModel);

          if( verbosityLevel() > 1 ) std::cout << spacing2 << "|ds| = " << norm(ds) << std::endl;
        }

        regularityTest(nu,tau);


        if( acceptanceTest != AcceptanceTest::LeftAdmissibleDomain ) acceptanceTest = acceptedSteps(norm_x,norm_dx,eta);

        if( acceptanceTest == AcceptanceTest::TangentialStepFailed && omegaL < (1 + 0.25 * (1 - etaMin)) * omegaL.previous() )
        {
          if( verbosityLevel() > 1 ) std::cout << spacing2 << "Stagnating update of omegaL. Accepting Step." << std::endl;
          acceptanceTest = AcceptanceTest::Passed;

          if( eta < rejectionTolerance)
          {
            if( verbosityLevel() > 1 ) std::cout << spacing2 << "Ignoring tangential step." << std::endl;
            trial -= tau*Dt;
            dx -= tau*Dt;
            norm_dx = norm(dx);
          }
        }

        if( acceptanceTest == AcceptanceTest::Passed )
        {
          norm_dx_old = norm_dx;
        }

        if( verbosityLevel() > 1 )
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
      if( tangentialSolver != nullptr && !tangentialSolver->isPositiveDefinite() ) return false;
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
      if( N_ == nullptr ) return;
      if( norm_dx < sqrtEps() * norm_x ) return;
      setContraction( norm_ds/norm_dx );
      //    if( contraction() < 0.25 && ( norm_dx < sqrtEps() * norm_x || norm_ds < eps() * norm_x ) ) return;

      if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) || omegaC < 2*contraction()/norm_dx )
        omegaC = 2*contraction()/norm_dx;

      if( verbosityLevel() > 1 ) std::cout << spacing2 << "theta = " << contraction() << ", omegaC: " << omegaC << std::endl;
    }

    double AffineCovariantCompositeSteps::updateOmegaL(const Vector& soc, double q_tau,
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

      if( verbosityLevel() > 1 )
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
      if( N_ == nullptr ) return 1;
      DampingFactor nu = 1;
      if( norm_Dn > eps() && std::abs(norm_Dn*omegaC) > eps() ) nu = std::min(1.,desiredContraction()/(omegaC*norm_Dn));
      return nu;
    }

    double AffineCovariantCompositeSteps::computeTangentialStepDampingFactor(double norm_dn, double norm_Dt, const CompositeStep::CubicModel& cubic) const
    {
      if( L_ == nullptr ) return 1;
      if( norm_Dt < sqrtEps() ) return 1;

      auto maxTau = 1.;
      if( pow(relaxedDesiredContraction()/omegaC,2) - norm_dn*norm_dn > 0)
        maxTau = std::min( 1. , sqrt( pow( 2*relaxedDesiredContraction()/omegaC , 2 ) - norm_dn*norm_dn )/norm_Dt );

      DampingFactor tau = CompositeStep::findMinimizer( cubic, 0, maxTau , dampingTolerance );

      return tau;
    }

    AffineCovariantCompositeSteps::AcceptanceTest AffineCovariantCompositeSteps::acceptedSteps(double norm_x, double norm_Dx, double eta)
    {
      if( norm_Dx < eps() * norm_x ) return AcceptanceTest::Passed;

      if( L_ != nullptr && eta < etaMin )
      {
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting tangential step." << std::endl;
        tangentialStepMonitor = StepMonitor::Rejected;
        return AcceptanceTest::TangentialStepFailed;
      }

      if( N_ != nullptr && !admissibleContraction() )
      {
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting normal step: " << contraction() << std::endl;
        normalStepMonitor = StepMonitor::Rejected;
        return AcceptanceTest::NormalStepFailed;
      }

      return AcceptanceTest::Passed;
    }

    void AffineCovariantCompositeSteps::regularityTest(double nu, double tau) const
    {
      if( !regularityTestPassed(nu) ) throw RegularityTestFailedException("AffineCovariantCompositeSteps::regularityTest (nu,...)",nu);
      if( !regularityTestPassed(tau) ) throw RegularityTestFailedException("AffineCovariantCompositeSteps::regularityTest (...,tau)",tau);
    }
  }
}
