#include "acr.hh"

#include <Spacy/operator.hh>
#include <Spacy/Algorithm/dampingFactor.hh>
#include <Spacy/Algorithm/CG/linearSolver.hh>
#include <Spacy/Algorithm/CompositeStep/quadraticModel.hh>
#include <Spacy/Algorithm/CG/cg.hh>
#include <Spacy/Algorithm/Scalar/findGlobalMinimizer.hh>
#include <Spacy/Algorithm/CG/linearSolver.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/log.hh>

#include <cmath>
#include <iostream>
#include <utility>


namespace
{
    class TrivialPreconditioner
    {
    public:
        //compute A(x)
        Spacy::Vector operator()(Spacy::Vector x) const{ return x; }
    };
}


namespace Spacy
{
    namespace ACR
    {
        DEFINE_LOG_TAG( static const char* log_tag = "ACR" );
         
        CompositeStep::CubicModel makeCubicModel(const Vector& dx,
                                                 const C2Functional& f, const Vector& x, Spacy::Real omega)
        {
            return CompositeStep::CubicModel( CompositeStep::makeQuadraticModel(Spacy::DampingFactor(0.0),dx,dx,f,x), CompositeStep::makeQuadraticNormModel(Spacy::DampingFactor(0.0),dx,dx), omega );
        }

        ACRSolver::ACRSolver(C2Functional f,   double eta1,  double eta2, double epsilon, double relativeAccuracy,  double omegaMax, double lambdaMax)
            : Mixin::RelativeAccuracy(relativeAccuracy),
              f_(std::move(f)),
              domain_(f_.domain()),
              eta1_(eta1), eta2_(eta2),  epsilon_(epsilon),  omegaMax_(omegaMax),  lambdaMax_(lambdaMax)
        {}

        Vector ACRSolver::operator()()
        {
            return operator()( zero(domain_) );
        }

        Vector ACRSolver::operator()(const Vector& x0)
        {  
            LOG_INFO(log_tag, "Starting ACR-Solver.")
            
            auto x = x0;

            for(unsigned step = 1; step <= getMaxSteps(); ++step)
            {
                
                LOG_BEGIN_SECTION
                LOG(log_tag, "Iteration", step)
                
                stepMonitor = StepMonitor::Accepted;
                // TODO domain_.setScalarProduct( );
                Real lambda = 1.0;
        
                do
                {
                    auto dx = computeStep(x);
                    auto cubicModel = makeCubicModel(dx,f_,x,omega_);
                    LOG_INFO(log_tag, "Computing damping factor")

                    lambda = Scalar::findGlobalMinimizer(cubicModel, 0, 1, 1e-2);

                    dx = lambda*dx;

                    LOG(log_tag, "lambda: ", lambda, " omega: ",  omega_, " cubicModel: ", cubicModel(lambda))


                    if( (stepMonitor = acceptanceTest(x,dx,lambda,cubicModel)) == StepMonitor::Accepted )
                    {
                        x += dx;
                        //  if( convergenceTest(/*TODO*/) ) return x;
                        TrivialPreconditioner trivialP;
                        // Todo: Change Norm
                        if (f_.d1(x)(trivialP(f_.d1(x))) < epsilon_ * epsilon_)
                            return x;
                    }

                    else LOG_INFO(log_tag, "Rejected")

                    // Modifikation von omega
                    omega_ = weightChange(omega_);

                    if ( omega_ > omegaMax_ )
                        return x;
                }
                // end iteration
                while( stepMonitor == StepMonitor::Rejected || lambda < lambdaMax_ );
                LOG_END_SECTION(log_tag)
            }

            return x;
        }

        ACRSolver::StepMonitor ACRSolver::acceptanceTest(const Vector &x, const Vector &dx, const Real & lambda, const CompositeStep::CubicModel& cubicModel)
        {
            
            // Prevent dividing by zero e.g. in the case x_0 = opimal solution
            // Consider the Case numerator = nominator = 0
            
            auto diff = cubicModel(lambda)-cubicModel(0.0);
                
            if (abs(diff) < eps() ) 
            {
                if ( diff < 0) diff = -eps();
                     
                else  diff = eps();
            }
            
            rho_ = (f_(x+dx)-f_(x))/diff;

            LOG(log_tag, "f_(x+dx): ",  f_(x+dx),  "f_(x): ",  f_(x), "CubicModel(lambda): ",  cubicModel(lambda), "CubicModel(0): ",  cubicModel(0), "rho: ", rho_,  "eta1 ",  eta1_,  "eta2 ",  eta2_ )
         
            if (rho_ >= eta1_)
                return StepMonitor::Accepted;

            return StepMonitor::Rejected;
        }

        Spacy::Real ACRSolver::weightChange(Spacy::Real omega) const
        {
            
            LOG(log_tag, "rho: ",  rho_,  "eta1 ",  eta1_,  "eta2 ",  eta2_  )

            if ( rho_ > eta2_ )
                omega *= 0.5;
            else if ( rho_ < eta1_ )
                omega *= 2;

            return omega;
        }

        Vector ACRSolver::computeStep(const Spacy::Vector &x) const
        {
            LinearSolver preconditioner = {};
            preconditioner = f_.hessian(x)^-1;
                        
            auto & cgSolver = cast_ref<CG::LinearSolver>(preconditioner);
            auto tcg =  makeTCGSolver( f_.hessian(x) , cgSolver.P());
//
//             auto tcg =  makeTCGSolver( f_.hessian(x) , f_.hessian(x).solver());
            tcg.setRelativeAccuracy(getRelativeAccuracy());

            return tcg(-f_.d1(x));
        }
    }
}
