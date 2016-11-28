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
        CompositeStep::CubicModel makeCubicModel(const Vector& dx,
                                                 const C2Functional& f, const Vector& x, Spacy::Real omega)
        {
            return CompositeStep::CubicModel( CompositeStep::makeQuadraticModel(Spacy::DampingFactor(0.0),dx,dx,f,x), CompositeStep::makeQuadraticNormModel(Spacy::DampingFactor(0.0),dx,dx), omega );
        }

        ACRSolver::ACRSolver(C2Functional f,   double eta1,  double eta2, double epsilon, double relativeAccuracy,  double omegaMax)
            : Mixin::RelativeAccuracy(relativeAccuracy),
              f_(std::move(f)),
              domain_(f_.domain()),
              eta1_(eta1), eta2_(eta2),  epsilon_(epsilon),  omegaMax_(omegaMax)
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

                if ( verbose() )
                    std::cout << "\nACR: Iteration " << step << ".\n";

                do
                {
                    auto dx = computeStep(x);

                    auto cubicModel = makeCubicModel(dx,f_,x,omega_);

                    if( verbose() )
                        std::cout << "Computing damping factor." << std::endl;
                    lambda = Scalar::findGlobalMinimizer(cubicModel, 0, 1, 1e-2);

                    dx = lambda*dx;

                    if( verbose() )
                        std::cout << "lambda: " << lambda << " omega: " << omega_ << " cubicModel: " << cubicModel(lambda) << std::endl;


                    if( (stepMonitor = acceptanceTest(x,dx,lambda,cubicModel)) == StepMonitor::Accepted )
                    {
                        x += dx;
                        //  if( convergenceTest(/*TODO*/) ) return x;
                        TrivialPreconditioner trivialP;
                        // Todo: Change Norm
                        if (f_.d1(x)(trivialP(f_.d1(x))) < epsilon_ * epsilon_)
                            return x;
                    }

                    else if( verbose() )
                        std::cout << "Rejected!" << std::endl;

                    // Modifikation von omega
                    omega_ = weightChange(omega_);

                    if ( omega_ > omegaMax_ )
                        return x;
                }
                // end iteration
                while( stepMonitor == StepMonitor::Rejected || lambda < 0.01 );

            }

            return x;
        }

        ACRSolver::StepMonitor ACRSolver::acceptanceTest(const Vector &x, const Vector &dx, const Real & lambda, const CompositeStep::CubicModel& cubicModel)
        {
            auto diff = cubicModel(lambda) - cubicModel(0);

            auto sign = diff / abs(diff);
            diff = sign * max( abs(diff), eps() );
            // 		rho_ = (f_(x+dx)-f_(x))/std::min(cubicModel(lambda)-cubicModel(0.0),Spacy::Real(eps()));

            rho_ = (f_(x+dx)-f_(x))/diff;

            if ( verbose() )
            {
                std::cout << "f_(x+dx): " << f_(x+dx) << "f_(x): " << f_(x) <<std::endl;
                std::cout << "CubicModel(0): " << cubicModel(0) << std::endl;
                std::cout << "rho: " << rho_ << "eta1 " << eta1_ << "eta2 " << eta2_ << std::endl;
            }

            if (rho_ >= eta1_)
                return StepMonitor::Accepted;

            return StepMonitor::Rejected;
        }

        Spacy::Real ACRSolver::weightChange(Spacy::Real omega) const
        {
            if ( verbose() )
                std::cout << "rho: " << rho_ << "eta1 " << eta1_ << "eta2 " << eta2_ << std::endl;

            if ( rho_ > eta2_ )
                omega *= 0.5;
            else if ( rho_ < eta1_ )
                omega *= 2;

            return omega;
        }

        Vector ACRSolver::computeStep(const Spacy::Vector &x) const
        {
            auto tcg =  makeTCGSolver( f_.hessian(x) , f_.hessian(x).solver());
            tcg.setRelativeAccuracy(getRelativeAccuracy());

            return tcg(f_.d1(x));
        }
    }
}
