
#include "acr.hh"

#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/Algorithm/CG/linearSolver.hh"
#include "Spacy/Algorithm/CG/triangularStateConstraintPreconditioner.hh"
#include "Spacy/Algorithm/CompositeStep/quadraticModel.hh"
#include "Spacy/Algorithm/CG/cg.hh"
#include "Spacy/Algorithm/Scalar/findGlobalMinimizer.hh"
#include  "Spacy/Algorithm/CG/linearSolver.hh"
#include <Spacy/zeroVectorCreator.hh>
#include "Spacy/inducedScalarProduct.hh"
#include "Spacy/Util/Exceptions/regularityTestFailedException.hh"
#include "Spacy/Util/cast.hh"
#include <Spacy/Util/Mixins/get.hh>

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
      return CompositeStep::CubicModel( CompositeStep::makeQuadraticModel(Spacy::DampingFactor(0.0),dx,dx,f,x), CompositeStep::makeQuadraticNormModel(Spacy::DampingFactor(0.0),dx,dx), omega );
    }	 
	 
    ACRSolver::ACRSolver(C2Functional f,   double eta1,  double eta2, double epsilon, double relativeAccuracy,  double omegaMax)
      : f_(std::move(f)),
        domain_(f_.domain()),
        eta1_(eta1), eta2_(eta2),  epsilon_(epsilon),  relativeAccuracy_(relativeAccuracy), omegaMax_(omegaMax)
    {}
    
    Vector ACRSolver::operator()()
    {
      return operator()( zero(domain_) );
     
    }

    Vector ACRSolver::operator()(const Vector& x0)
    {
        
      auto x = x0;

      for(unsigned step = 0; step < getMaxSteps(); ++step)
      {
   
        stepMonitor = StepMonitor::Accepted;

        // TODO domain_.setScalarProduct( );
        
		Real lambda = 1.0;
		
		
       if ( verbose() ) std::cout << "\nACR: Iteration " << step << ".\n";
        
        do
        {
                        auto dx = computeStep(x);  

			CompositeStep::CubicModel cubicModel = makeCubicModel(dx,f_,x,omega_);
	
			lambda = findGlobalMinimizer(cubicModel, 0, 1, 1e-2);

			dx = lambda*dx;

                        if( verbose() )
			{
       
                            std::cout << std::setw(14) << "lambda: " << std::setw(14) << lambda << std::setw(14) << " omega: " << std::setw(14) << omega_ << std::setw(14) << " cubicModel: " << std::setw(14) << cubicModel(lambda) << std::endl;
                        }
 
			if( verbose() )
			{
				//   std::cout << spacing << "Step length: " << norm(dx) << std::endl;
				std::cout << std::setw(14) << "Computing damping factors." << std::endl;
			}

			// Akzeptanztest 
        
			stepMonitor = acceptanceTest(x,dx,lambda,cubicModel);
         
        
			if(stepMonitor == StepMonitor::Accepted)
			{
				x += dx;
				//  if( convergenceTest(/*TODO*/) ) return x;
				TrivialPreconditioner trivialP;
                                // Todo: Change Norm
				if (f_.d1(x)(trivialP(f_.d1(x))) < epsilon_ * epsilon_)
					return x;
			}
			
			else
			{  
                           if( verbose() )
			   {
                              std::cout << "Rejected!" << std::endl;
                           }
                        
			}
			// Modifikation von omega
			omega_ = ACRSolver::weightChange(dx, x, omega_);
			
			if ( omega_ > omegaMax_ ) return x;

		} 
  // end iteration
		while(stepMonitor == StepMonitor::Rejected || lambda < 0.01  );
		
      }

      return x;
    }

    ACRSolver::StepMonitor ACRSolver::acceptanceTest(const Vector &x, const Vector &dx, const Real & lambda, const CompositeStep::CubicModel& cubicModel) 
    {
                  //"cubicModel(0)=f_(x)"
                
                Real diff(cubicModel(lambda)-cubicModel(0.0));
                
                if (abs(diff) < 1e-16) 
                {
                   if ( diff < 0) diff = -1e-16;
                     
                   else  diff = 1e-16;
                }
                  
// 		rho_ = (f_(x+dx)-f_(x))/std::min(cubicModel(lambda)-cubicModel(0.0),Spacy::Real(1e-16));

                rho_ = (f_(x+dx)-f_(x))/diff;
                
                if ( verbose() ) 
                {
                    std::cout << std::setw (14) << "f_(x+dx): " << std::setw (14) << f_(x+dx) << std::setw (14) << "f_(x): " << std::setw (14) << f_(x) <<std::endl;
                    std::cout << std::setw(14) << "CubicModel(0): " << std::setw(14) << cubicModel(0) << std::endl;
                    std::cout << std::setw(14) << "rho: " << std::setw(14) << rho_ << std::setw(14) << "eta1 " << std::setw(14) << eta1_ << std::setw (14) << "eta2 " << std::setw(14) << eta2_ << std::endl;
                }

		if (rho_ >= eta1_) return StepMonitor::Accepted;
      
		return StepMonitor::Rejected;
    }
    
    Spacy::Real ACRSolver::weightChange(Spacy::Vector dx, Spacy::Vector x, Spacy::Real omega) const
    {
		if ( verbose() )
                { 
                    std::cout << std::setw(14) << "rho: " << std::setw(14) << rho_ << std::setw(14) << "eta1 " << std::setw(14) << eta1_ << std::setw(14) << "eta2 " << std::setw(14) << eta2_ << std::endl;
                }  
		if ( rho_ > eta2_ )
                {
                    omega *= 0.5;
                }	        
                
                else
		{      
                        if ( rho_ < eta1_ )
                        {
                           omega *= 2.;
                        }
                }
                
		return omega;
	}

    Vector ACRSolver::computeStep(const Spacy::Vector &x) const
    {
			Spacy::Vector z = f_.d1(x);
                        preconditioner = {};
                        preconditioner = f_.hessian(x)^-1;
                        
                        auto & cgSolver = cast_ref<CG::LinearSolver>(preconditioner);
                        
                        // @Lars: Why does passing the preconditioner by   auto tcg =  makeTCGSolver( f_.hessian(x) , f_.hessian(x).solver()) not work ?
 			auto tcg =  makeTCGSolver( f_.hessian(x) , cgSolver.P());
//                         auto tcg =  makeTCGSolver( f_.hessian(x) , f_.hessian(x).solver());
			
                        tcg.setRelativeAccuracy(relativeAccuracy_);
                        
                        // Add Initial Guess for CG
			auto dx = tcg(-z);

             return dx;
	
	}
			

  }
}
