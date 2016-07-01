
#include "acr.hh"

#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/Algorithm/CG/linearSolver.hh"
#include "Spacy/Algorithm/CG/triangularStateConstraintPreconditioner.hh"
#include "Spacy/Algorithm/CompositeStep/quadraticModel.hh"
#include "Spacy/Algorithm/CG/cg.hh"
#include "Spacy/Algorithm/Scalar/findGlobalMinimizer.hh"
#include  "Spacy/Algorithm/CG/linearSolver.hh"

#include "Spacy/inducedScalarProduct.hh"
#include "Spacy/Util/Exceptions/regularityTestFailedException.hh"

#include <boost/type_erasure/is_empty.hpp>

#include <cmath>
#include <iostream>
#include <utility>



namespace Spacy
{
  namespace ACR
 {
    CompositeStep::CubicModel makeCubicModel(const Vector& dx,
                              const C2Functional& f, const Vector& x, Spacy::Real omega)
    {
      return CompositeStep::CubicModel( CompositeStep::makeQuadraticModel(0,dx,dx,f,x), CompositeStep::makeQuadraticNormModel(0,dx,dx), omega );
    }
	 
	 
    ACRSolver::ACRSolver(C2Functional f)
      : f_(std::move(f)),
        domain_(f_.domain()),
        rho(1), eta1(0.25),eta2(0.5),gamma1(1.5),gamma2(2.0)
    {}

    Vector ACRSolver::operator()()
    {
      return operator()( domain_.zeroVector() );
     
    }

    Vector ACRSolver::operator()(const Vector& x0)
    {
      auto lastStepWasUndamped = false;
      auto x = x0;

      for(unsigned step = 1; step < getMaxSteps(); ++step)
      {
        stepMonitor = StepMonitor::Accepted;

        // TODO domain_.setScalarProduct( );
        
		Real lambda = 1.0;

        do
        {

			if( verbose() ) std::cout << "\nComposite Steps: Iteration " << step << ".\n";
			if( verbose() ) std::cout << spacing << "Computing step." << std::endl;
			auto dx = computeStep(x);  // Näherungsweise Lösung des kubischen Modellproblems

			CompositeStep::CubicModel cubicModel = makeCubicModel(dx,f_,x,omega);
	
			lambda = findGlobalMinimizer(cubicModel, 0, 1, 1e-2);

			dx *= toDouble(lambda);


			std::cout << "lambda: " << lambda << " cubicModel: "<< cubicModel(lambda) << std::endl;
 
			if( verbose() )
			{
				//   std::cout << spacing << "Step length: " << norm(dx) << std::endl;
				std::cout << spacing << "Computing damping factors." << std::endl;
			}

			// Akzeptanztest 
        
			stepMonitor = acceptanceTest(x,dx,lambda,cubicModel);
         
        
			if(stepMonitor == StepMonitor::Accepted)
			{
				std::cout << "Akzeptiert:" << cast_ref<Spacy::Rn::Vector>(x).get() << "+" << cast_ref<Spacy::Rn::Vector>(dx).get() << std::endl;
				x += dx;
				//  if( convergenceTest(/*TODO*/) ) return x;
				double epsilon=0.0000001;
				TrivialPreconditioner trivialP;

				if (f_.d1(x)(trivialP(f_.d1(x)))<epsilon*epsilon)
					return x;

			}
			else
			{
				std::cout << "Verworfen:" << cast_ref<Spacy::Rn::Vector>(x).get() << "+" << cast_ref<Spacy::Rn::Vector>(dx).get() << std::endl;
			}
			
			// Modifikation von omega
			Spacy::Real omega = ACRSolver::weightChange(dx, x, omega);

		} 
		while(stepMonitor == StepMonitor::Rejected || lambda < 0.01); // end iteration
		
      }

      return x;
    }

    ACRSolver::StepMonitor ACRSolver::acceptanceTest(const Vector &x, const Vector &dx, Real lambda, const CompositeStep::CubicModel& cubicModel) const
    {
		//if(f_(x+dx) <= f_(x)+0.1*f_.d1(x)(dx)) return StepMonitor::Accepted;
		//return StepMonitor::Rejected;
		
		
		Spacy::Real rho=(f_(x+dx)-f_(x))/(cubicModel(lambda)-cubicModel(0)); //"cubicModel(0)=f_(x)"

		std:: cout << f_(x+dx) << " " << f_(x) << " " << cubicModel(lambda) << " " << cubicModel(0) << std::endl;


		if (rho >= eta1) return StepMonitor::Accepted;
		return StepMonitor::Rejected;
    }
    
    Spacy::Real ACRSolver::weightChange(Spacy::Vector dx, Spacy::Vector x, Spacy::Real omega_) const
    {
		
		if (rho>eta2)
			Spacy::Real omega_=1/2*omega_;
		else
		{
			if (eta1<=rho && rho<=eta2) 
				Spacy::Real omega_=omega_;
			else
				Spacy::Real omega_=2*omega_;
			
		}
		return omega_;
	}

    Vector ACRSolver::computeStep(const Spacy::Vector &x) const
    {
			//~ cast_ref<Spacy::Vector>(x).impl() = 2;
			Spacy::Real y=f_(x);
			Spacy::Vector z=f_.d1(x);
			
			TrivialPreconditioner trivialP;
			
			auto tcg =  makeTCGSolver( f_.hessian(x) , trivialP);
			
						
			Spacy::Vector dx=domain_.zeroVector();
			
			dx=tcg(-z);
			
			std::cout << "f_(x)=" << y << std::endl << "dx" << cast_ref<Spacy::Rn::Vector>(dx).get() << std::endl; // << "f_''(x)=" << w.impl() << std::endl;
		
		     // Try to solve a linear system by tcg Input: z=f_.d1(x), hessian, Output dx    
		
             return dx;

		//if( is_empty(L_) ) return Vector(0*x);

		////solver = makeSolver(nu,x,lastStepWasUndamped);
	
	}
			
      //// Hier die Lösung des Kubischen Problems berechnen, wir wissen, dass x zunächst reell ist.
      //// min f(x)+f'(x)v+1/2 b(v,v) + omega/6 \|v\|^3  \|v\|^2 = < v , v >
						//// v_opt=-(2+2b)/omega;           ->b(v,v)=v*b*v=b*v²
						//// alpha= -f'(x)/b(v,v); oder WP-Liniensuche
		////return x+alpha*v_opt;
////      return solver(  );
    //}

    //IndefiniteLinearSolver ACRSolver::makeSolver(const Vector &x, bool lastStepWasUndamped) const
    //{
      //Real trcgRelativeAccuracy = minimalAccuracy();
      //if( nu == 1 && lastStepWasUndamped )
      //{
        //trcgRelativeAccuracy = max( relativeAccuracy() , min( minimalAccuracy() , omegaL() * norm_dx_old ) );
        //if( norm_dx_old > 0 && lastStepWasUndamped )
          //trcgRelativeAccuracy = min( max( relativeAccuracy()/norm_dx_old , trcgRelativeAccuracy ) , minimalAccuracy() );
        //if( verbosityLevel() > 1 )
        //{
          //std::cout << spacing2 << "relative accuracy = " << trcgRelativeAccuracy << std::endl;
          //std::cout << spacing2 << "absolute step length accuracy = " << relativeAccuracy()*norm(x) << std::endl;
        //}
      //}

      //auto setParams = [this,&x](auto& solver)
      //{
        //solver.setIterativeRefinements(iterativeRefinements());
        //solver.setVerbosityLevel( verbosityLevel() );
        //if( norm(primal(x)) > 0)
          //solver.setAbsoluteAccuracy( relativeAccuracy()*norm(primal(x)) );
        //else
          //solver.setAbsoluteAccuracy( eps() );
        //solver.setMaxSteps(maxSteps());
      //};

  ////    std::unique_ptr<CGSolver> trcg = nullptr;

      //if( is<CG::LinearSolver>(normalSolver) )
      //{
        //const auto& cgSolver = cast_ref<CG::LinearSolver>(normalSolver);
        //if( is<CG::TriangularStateConstraintPreconditioner>(cgSolver.P()))
        //{
          //auto trcg =  makeTRCGSolver( L_.hessian(x) , cgSolver.P() ,
                                       //toDouble(trcgRelativeAccuracy) , eps() , verbose() );
          //setParams(trcg);
          //return IndefiniteLinearSolver(trcg);
        //}
      //}

  ////    if( trcg == nullptr )
       //auto trcg = makeTRCGSolver( L_.hessian(x) , normalSolver ,
                                   //toDouble(trcgRelativeAccuracy) , eps(), verbose() );
  ////    trcg.setIterativeRefinements(iterativeRefinements());
  ////    trcg.setDetailedVerbosity(verbose_detailed());
  ////    if( norm(primal(x)) > 0)
  ////      trcg.setAbsoluteAccuracy( relativeAccuracy()*norm(primal(x)) );
  ////    else
  ////      trcg.setAbsoluteAccuracy( eps() );
  ////    trcg.setMaxSteps(maxSteps());
       //setParams(trcg);
      //return IndefiniteLinearSolver(trcg);
      ////return std::move(trcg);
  ////    return std::unique_ptr<IndefiniteLinearSolver>( trcg.release() );
    //}



    //Real ACRSolver::updateOmega(const Vector& soc, Real q_tau,
                                                       //Real tau, Real norm_x, Real norm_dx, const CompositeStep::CubicModel& cubic)
    //{
      //if( is_empty(tangentialSolver) ) return 1;

      //Real eta = 1;
      //if( abs( cubic(tau) - cubic(0) ) > sqrtEps()*norm_x )
        //eta = ( L_(primal(soc)) - cubic(0) )/( cubic(tau) - cubic(0) );
      //else eta = 1;

      //if( !(normalStepMonitor == StepMonitor::Rejected && tangentialStepMonitor == StepMonitor::Rejected) ||
          //omegaL < (L_(primal(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx) )
        //omegaL = (L_(primal(soc)) - q_tau)*6/(norm_dx*norm_dx*norm_dx);

      //if( verbosityLevel() > 1 )
      //{
        //std::cout << spacing2 << "predicted decrease: " << (cubic(tau)-cubic(0)) << std::endl;
        //std::cout << spacing2 << "L(primal(soc)): " << L_(primal(soc)) << ", |primal(soc)| = " << norm(primal(soc)) << std::endl;
        //std::cout << spacing2 << "actual decrease: " << ( L_(primal(soc)) - cubic(0) ) << std::endl;
        //std::cout << spacing2 << "omegaL: " << omegaL << std::endl;
        //std::cout << spacing2 << "eta: " << eta << std::endl;
      //}

      //return eta;
    //}



    //Real ACRSolver::computeTangentialStepDampingFactor(Real norm_dn, Real norm_Dt, const CompositeStep::CubicModel& cubic) const
    //{
      //if( is_empty(L_) ) return 1;
      //if( norm_Dt < sqrtEps() ) return 1;

      //auto maxTau = Real{1.};
      //if( pow(relaxedDesiredContraction()/omegaC,2) - norm_dn*norm_dn > 0)
        //maxTau = min( 1. , sqrt( pow( 2*relaxedDesiredContraction()/omegaC , 2 ) - norm_dn*norm_dn )/norm_Dt );

      //return CompositeStep::findMinimizer( cubic, 0, maxTau , dampingAccuracy() );
    //}

    //ACRSolver::AcceptanceTest ACRSolver::acceptedSteps(Real norm_x, Real norm_Dx, Real eta)
    //{
      //if( norm_Dx < eps() * norm_x ) return AcceptanceTest::Passed;

      //if( !is_empty(L_) && !acceptableDecrease( eta ) )
      //{
        //if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting tangential step." << std::endl;
        //tangentialStepMonitor = StepMonitor::Rejected;
        //return AcceptanceTest::TangentialStepFailed;
      //}

      //if( !is_empty(N_) && !admissibleContraction() )
      //{
        //if( verbosityLevel() > 1 ) std::cout << spacing2 << "Rejecting normal step: " << contraction() << std::endl;
        //normalStepMonitor = StepMonitor::Rejected;
        //return AcceptanceTest::NormalStepFailed;
      //}

      //return AcceptanceTest::Passed;
    //}

    //void ACRSolver::regularityTest(Real nu, Real tau) const
    //{
      //if( !regularityTestPassed(nu) ) throw RegularityTestFailedException("AffineCovariantSolver::regularityTest (nu,...)",toDouble(nu));
      //if( !regularityTestPassed(tau) ) throw RegularityTestFailedException("AffineCovariantSolver::regularityTest (...,tau)",toDouble(tau));
    //}
  }
}
