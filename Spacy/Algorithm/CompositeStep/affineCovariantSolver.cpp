// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "affineCovariantSolver.hh"

#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/Algorithm/CG/linearSolver.hh"
#include "Spacy/Algorithm/CG/triangularStateConstraintPreconditioner.hh"
#include "Spacy/Algorithm/CompositeStep/quadraticModel.hh"

#include "Spacy/inducedScalarProduct.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Util/Exceptions/regularityTestFailedException.hh"

#include <cmath>
#include <iostream>
#include <utility>

namespace Spacy
{
  namespace
  {
    auto primalProjection(const Spacy::Vector& v)
    {
      auto w = v;
      auto& w_ = cast_ref<ProductSpace::Vector>(w);
      w_.component(DUAL) *= 0;
      return w;
    }

    auto dualProjection(const Spacy::Vector& v)
    {
      auto w = v;
      auto& w_ = cast_ref<ProductSpace::Vector>(w);
      w_.component(PRIMAL) *= 0;
      return w;
    }
  }


  namespace CompositeStep
  {
    enum class AffineCovariantSolver::AcceptanceTest{ Passed, Failed, LeftAdmissibleDomain, TangentialStepFailed, NormalStepFailed };

    AffineCovariantSolver::AffineCovariantSolver(C2Functional N, C2Functional L, VectorSpace& domain)
      : N_(std::move(N)),
        L_(std::move(L)),
        domain_(domain)
    {}

    Vector AffineCovariantSolver::operator()()
    {
      return operator()( domain_.zeroVector() );
    }

    Vector AffineCovariantSolver::operator()(const Vector& x0)
    {
      auto lastStepWasUndamped = false;
      auto x = x0;

      for(unsigned step = 1; step < maxSteps(); ++step)
      {
        normalStepMonitor = tangentialStepMonitor = StepMonitor::Accepted;

        domain_.setScalarProduct( PrimalInducedScalarProduct( N_.hessian(primalProjection(x)) ) );

        if( verbose() ) std::cout << "\nComposite Steps: Iteration " << step << ".\n";
        if( verbose() ) std::cout << spacing << "Computing normal step." << std::endl;
        auto Dn = computeNormalStep(x);
        auto norm_Dn = norm(Dn);
        if( verbose() )
        {
          std::cout << spacing << "Normal step length: " << norm_Dn << std::endl;
          std::cout << spacing << "Computing normal damping factor" << std::endl;
        }
        Real nu = computeNormalStepDampingFactor(norm_Dn);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "|dn| = " << norm_Dn << ", nu = " << nu << std::endl;

        if( verbose() ) std::cout << spacing << "Computing lagrange multiplier." << std::endl;
        x += computeLagrangeMultiplier(x);

        if( verbose() ) std::cout << spacing << "Computing tangential step." << std::endl;
        auto Dt = computeTangentialStep(nu,x,Dn,lastStepWasUndamped);

        Real tau = 0.;
        Real norm_x = 0., norm_dx = 0.;
        auto ds = Dt; auto dx = Dt;

        if( verbose() )
        {
          std::cout << spacing << "Tangential step length: " << norm(Dt) << std::endl;
          std::cout << spacing << "Computing damping factors." << std::endl;
        }
        std::tie(tau,dx,ds,norm_x,norm_dx) = computeCompositeStep( nu , norm_Dn , x , Dn , Dt );

        x += primalProjection(dx);
        if( contraction() < 0.25 ) x += primalProjection(ds);

        norm_x = norm(primalProjection(x));

        if( nu == 1 && tau == 1 ) lastStepWasUndamped = true;
        if( convergenceTest(nu,tau,norm_x,norm_dx) ) return x;

        if( verbose() ) std::cout << spacing2 << "nu = " << nu << ", tau = " << tau << ", |dx| = " << norm_dx << std::endl;
        if( verbose() ) std::cout << spacing2 << "|x| = " << norm_x << std::endl;
        if( verbosityLevel() > 1) std::cout << spacing2 << "(Dn,Dt) = " << Dn*Dt/(norm_Dn*norm(Dt)) << std::endl;
      } // end iteration

      return x;
    }

    Vector AffineCovariantSolver::computeTangentialStep(Real nu, const Vector &x, const Vector& dn, bool lastStepWasUndamped) const
    {
      if( !L_ ) return Vector(0*x);

      tangentialSolver = makeTangentialSolver(nu,x,lastStepWasUndamped);

      return primalProjection( tangentialSolver( primalProjection(-d1(L_,x)) + primalProjection(-nu*d2(L_,x)(dn)) ) );
    }

    IndefiniteLinearSolver AffineCovariantSolver::makeTangentialSolver(Real nu, const Vector &x, bool lastStepWasUndamped) const
    {
      Real trcgRelativeAccuracy = minimalAccuracy();
      if( nu == 1 && lastStepWasUndamped )
      {
        trcgRelativeAccuracy = max( relativeAccuracy() , min( minimalAccuracy() , omegaL() * norm_dx_old ) );
        if( norm_dx_old > 0 && lastStepWasUndamped )
          trcgRelativeAccuracy = min( max( relativeAccuracy()/norm_dx_old , trcgRelativeAccuracy ) , minimalAccuracy() );
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
        if( norm(primalProjection(x)) > 0)
          solver.setAbsoluteAccuracy( relativeAccuracy()*norm(primalProjection(x)) );
        else
          solver.setAbsoluteAccuracy( eps() );
        solver.setMaxSteps(maxSteps());
      };

  //    std::unique_ptr<CGSolver> trcg = nullptr;

      if( is<CG::LinearSolver>(normalSolver) )
      {
        const auto& cgSolver = cast_ref<CG::LinearSolver>(normalSolver);
        if( is<CG::TriangularStateConstraintPreconditioner>(cgSolver.P()))
        {
          auto trcg =  makeTRCGSolver( L_.hessian(x) , cgSolver.P() ,
                                       toDouble(trcgRelativeAccuracy) , eps() , verbose() );
          setParams(trcg);
          return IndefiniteLinearSolver(trcg);
        }
      }

  //    if( trcg == nullptr )
       auto trcg = makeTRCGSolver( L_.hessian(x) , normalSolver ,
                                   toDouble(trcgRelativeAccuracy) , eps(), verbose() );
  //    trcg.setIterativeRefinements(iterativeRefinements());
  //    trcg.setDetailedVerbosity(verbose_detailed());
  //    if( norm(primalProjection(x)) > 0)
  //      trcg.setAbsoluteAccuracy( relativeAccuracy()*norm(primalProjection(x)) );
  //    else
  //      trcg.setAbsoluteAccuracy( eps() );
  //    trcg.setMaxSteps(maxSteps());
       setParams(trcg);
      return IndefiniteLinearSolver(trcg);
      //return std::move(trcg);
  //    return std::unique_ptr<IndefiniteLinearSolver>( trcg.release() );
    }

    Vector AffineCovariantSolver::computeNormalStep(const Vector &x) const
    {
      if( !N_ ) return Vector(0*x);

      normalSolver = N_.hessian(primalProjection(x))^-1;
      return computeMinimumNormCorrection(x);
    }

    Vector AffineCovariantSolver::computeSimplifiedNormalStep(const Vector &trial) const
    {
      if( !N_ ) return Vector(0*trial);
      return computeMinimumNormCorrection(trial);
    }

    Vector AffineCovariantSolver::computeMinimumNormCorrection(const Vector& x) const
    {
      auto rhs = dualProjection(-d1(L_,x));
      Vector dn0 = 0*x;
      if( is<CG::LinearSolver>(normalSolver) )
      {
        auto& cgSolver = cast_ref<CG::LinearSolver>(normalSolver);
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
      return dn0 + primalProjection( normalSolver( rhs ) );
    }

    Vector AffineCovariantSolver::computeLagrangeMultiplier(const Vector& x) const
    {
      if( !N_ || !L_ ) return Vector(0*x);
      return dualProjection( normalSolver( primalProjection(-d1(L_,x)) ) );
    }

    std::tuple<Real, Vector, Vector, Real, Real> AffineCovariantSolver::computeCompositeStep(Real& nu, Real norm_Dn,
                                                        const Vector& x, const Vector& Dn, const Vector& Dt)
    {
      auto norm_Dt = norm(Dt);
      if( verbosityLevel() > 1 ) std::cout << spacing2 << "|Dt| = " << norm_Dt << std::endl;
      auto cubic = CompositeStep::makeCubicModel( nu, Dn, Dt , L_ , x , omegaL );
      auto tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubic);

      auto ds = Vector{0*Dt};
      auto dx = ds;
      auto eta = Real{1.};
      auto norm_x = norm(x);
      auto norm_dx = Real{0.};
      AcceptanceTest acceptanceTest = AcceptanceTest::Failed;

      do
      {
        if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) nu *= 0.5;
        else nu = computeNormalStepDampingFactor(norm_Dn);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "nu = " << nu << std::endl;

        auto quadraticModel = CompositeStep::makeQuadraticModel(nu,Dn,Dt,L_,x);
        auto cubicModel = CompositeStep::makeCubicModel(nu, Dn, Dt, L_, x, omegaL);

        if( acceptanceTest == AcceptanceTest::LeftAdmissibleDomain ) tau *= 0.5;
        else tau = computeTangentialStepDampingFactor(nu*norm_Dn,norm_Dt,cubicModel);
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "tau = " << tau << std::endl;
        auto q_tau = quadraticModel(tau);

        dx = primalProjection(nu*Dn) + primalProjection(tau*Dt);
        norm_dx = norm(primalProjection(dx));
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

        if( acceptanceTest == AcceptanceTest::TangentialStepFailed && omegaL < (1 + 0.25 * (1 - minimalDecrease())) * omegaL.previous() )
        {
          if( verbosityLevel() > 1 ) std::cout << spacing2 << "Stagnating update of omegaL. Accepting Step." << std::endl;
          acceptanceTest = AcceptanceTest::Passed;

          if( !acceptableRelaxedDecrease( eta ) )
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

    bool AffineCovariantSolver::convergenceTest(Real nu, Real tau, Real norm_x, Real norm_dx)
    {
      if( tangentialSolver && !tangentialSolver.isPositiveDefinite() ) return false;
      if( nu < 1 || tau < 1 ) return false;

      if( norm_dx < relativeAccuracy() * norm_x || ( norm_x < eps() && norm_dx < eps() )  )
      {
        if( verbose() ) std::cout << spacing << "Terminating (convergent)." << std::endl;
        return true;
      }

      return false;
    }


    void AffineCovariantSolver::updateOmegaC(Real norm_x, Real norm_dx, Real norm_ds)
    {
      if( !N_ ) return;
      if( norm_dx < sqrtEps() * norm_x ) return;
      setContraction( norm_ds/norm_dx );
      //    if( contraction() < 0.25 && ( norm_dx < sqrtEps() * norm_x || norm_ds < eps() * norm_x ) ) return;

      if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) || omegaC < 2*contraction()/norm_dx )
        omegaC = 2*contraction()/norm_dx;

      if( verbosityLevel() > 1 ) std::cout << spacing2 << "theta = " << contraction() << ", omegaC: " << omegaC << std::endl;
    }

    Real AffineCovariantSolver::updateOmegaL(const Vector& soc, Real q_tau,
                                                       Real tau, Real norm_x, Real norm_dx, const CompositeStep::CubicModel& cubic)
    {
      if( !tangentialSolver ) return 1;

      Real eta = 1;
      if( abs( cubic(tau) - cubic(0) ) > sqrtEps()*norm_x )
        eta = ( L_(primalProjection(soc)) - cubic(0) )/( cubic(tau) - cubic(0) );
      else eta = 1;

      if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) ||
          omegaL < (L_(primalProjection(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx) )
        omegaL = (L_(primalProjection(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx);

      if( verbosityLevel() > 1 )
      {
        std::cout << spacing2 << "predicted decrease: " << (cubic(tau)-cubic(0)) << std::endl;
        std::cout << spacing2 << "L(primalProjection(soc)): " << L_(primalProjection(soc)) << ", |primalProjection(soc)| = " << norm(primalProjection(soc)) << std::endl;
        std::cout << spacing2 << "actual decrease: " << ( L_(primalProjection(soc)) - cubic(0) ) << std::endl;
        std::cout << spacing2 << "omegaL: " << omegaL << std::endl;
        std::cout << spacing2 << "eta: " << eta << std::endl;
      }

      return eta;
    }


    Real AffineCovariantSolver::computeNormalStepDampingFactor(Real norm_Dn) const
    {
      if( !N_ ) return 1;
      DampingFactor nu = 1.;
      if( norm_Dn > eps() && abs(norm_Dn*omegaC()) > eps() ) nu = min(1.,desiredContraction()/(omegaC()*norm_Dn));
      return nu;
    }

    Real AffineCovariantSolver::computeTangentialStepDampingFactor(Real norm_dn, Real norm_Dt, const CompositeStep::CubicModel& cubic) const
    {
      if( !L_ ) return 1;
      if( norm_Dt < sqrtEps() ) return 1;

      auto maxTau = Real{1.};
      if( pow(relaxedDesiredContraction()/omegaC,2) - norm_dn*norm_dn > 0)
        maxTau = min( 1. , sqrt( pow( 2*relaxedDesiredContraction()/omegaC , 2 ) - norm_dn*norm_dn )/norm_Dt );

      return CompositeStep::findMinimizer( cubic, 0, maxTau , dampingAccuracy() );
    }

    AffineCovariantSolver::AcceptanceTest AffineCovariantSolver::acceptedSteps(Real norm_x, Real norm_Dx, Real eta)
    {
      if( norm_Dx < eps() * norm_x ) return AcceptanceTest::Passed;

      if( !!L_ && !acceptableDecrease( eta ) )
      {
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting tangential step." << std::endl;
        tangentialStepMonitor = StepMonitor::Rejected;
        return AcceptanceTest::TangentialStepFailed;
      }

      if( !!N_ && !admissibleContraction() )
      {
        if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting normal step: " << contraction() << std::endl;
        normalStepMonitor = StepMonitor::Rejected;
        return AcceptanceTest::NormalStepFailed;
      }

      return AcceptanceTest::Passed;
    }

    void AffineCovariantSolver::regularityTest(Real nu, Real tau) const
    {
      if( !regularityTestPassed(nu) ) throw RegularityTestFailedException("AffineCovariantSolver::regularityTest (nu,...)",toDouble(nu));
      if( !regularityTestPassed(tau) ) throw RegularityTestFailedException("AffineCovariantSolver::regularityTest (...,tau)",toDouble(tau));
    }
  }
}
