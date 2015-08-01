#include "newton.hh"

#include "linearOperator.hh"
#include "functionSpaceElement.hh"

#include <iostream>

namespace Algorithm
{
  namespace Newton
  {
    NewtonMethod::NewtonMethod(const C1Operator& F)
      : F_(F),
        dampingFactor_(std::make_unique<DampingStrategy::AffineCovariant>(F_)),
        terminationCriterion_( std::make_unique<TerminationCriterion::AffineCovariant>(F_,relativeAccuracy()) )
    {
      connectEps( *dampingFactor_ );
      connectRegularityTest( *dampingFactor_ );
      connectEps( *terminationCriterion_ );
      connectVerbosity( *terminationCriterion_ );
      connectRelativeAccuracy( *terminationCriterion_ );
    }

    FunctionSpaceElement NewtonMethod::solve() const
    {
      return solve( FunctionSpaceElement( F_.impl().domain().element() ) );
    }

    FunctionSpaceElement NewtonMethod::solve(const FunctionSpaceElement& x0) const
    {
      using namespace std::chrono;
      if( verbose() ) std::cout << "Starting newton iteration." << std::endl;
      startTimer();

      auto x = x0;
      for(unsigned i = 1; i <= maxSteps(); ++i)
      {
        if( verbose() ) std::cout << "Iteration " << i << ": ";

        auto DF_Inv = F_.getLinearization(x)^-1;

        auto dx = DF_Inv(-F_(x));
        auto nu = dampingFactor_->compute(DF_Inv,x,dx);
        x += static_cast<double>(nu)*dx;

        if( verbose() ) std::cout << "nu = " << nu << ", |x| = " << norm(x) << ", |dx| = " << norm(dx) << std::endl;

        if( terminationCriterion_->passed(nu,x,dx) )
        {
          if( verbose() ) std::cout << "Newton iteration converged. Computation time: " << elapsedTime() << "s." << std::endl;
          return x;
        }
      }

      return x;
    }
  }

  Newton::NewtonMethod localNewton(const C1Operator& F)
  {
    using namespace Newton;
    auto newton = NewtonMethod( F );
    newton.setDampingStrategy<DampingStrategy::None>();
    return newton;
  }

  Newton::NewtonMethod covariantNewton(const C1Operator& F)
  {
    using namespace Newton;
    return NewtonMethod( F );
  }

  Newton::NewtonMethod contravariantNewton(const C1Operator& F)
  {
    using namespace Newton;
    auto newton = NewtonMethod( F );
    newton.setDampingStrategy<DampingStrategy::AffineContravariant>();
    newton.setTerminationCriterion<TerminationCriterion::AffineContravariant>();
    return newton;
  }
}
