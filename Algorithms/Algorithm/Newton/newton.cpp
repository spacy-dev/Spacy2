#include "newton.hh"

#include "linearOperator.hh"
#include "vector.hh"

#include <iostream>

namespace Algorithm
{
  namespace Newton
  {
    NewtonMethod::NewtonMethod(const Operator& F)
      : F_(F),
        dampingFactor_(std::make_shared<DampingStrategy::AffineCovariant>(F_)),
        terminationCriterion_( std::make_shared<TerminationCriterion::AffineCovariant>(F_,relativeAccuracy()) )
    {
      attachEps( dampingFactor_.get() );
      attachRegularityTest( dampingFactor_.get() );
      attachEps( terminationCriterion_.get() );
      attachVerbosity( terminationCriterion_.get() );
      attachRelativeAccuracy( terminationCriterion_.get() );
    }

    Vector NewtonMethod::solve() const
    {
      return solve( Vector( F_.impl().domain().element() ) );
    }

    Vector NewtonMethod::solve(const Vector& x0) const
    {
      using namespace std::chrono;
      if( verbose() ) std::cout << "Starting newton iteration." << std::endl;
      startTimer();

      auto x = x0;
      for(unsigned i = 1; i <= maxSteps(); ++i)
      {
        if( verbose() ) std::cout << "\nIteration " << i << ": ";

        auto DF_Inv = F_.linearization(x)^-1;

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

  Newton::NewtonMethod localNewton(const Operator& F)
  {
    using namespace Newton;
    auto newton = NewtonMethod( F );
    newton.setDampingStrategy<DampingStrategy::None>();
    return newton;
  }

  Newton::NewtonMethod covariantNewton(const Operator& F)
  {
    using namespace Newton;
    return NewtonMethod( F );
  }

  Newton::NewtonMethod contravariantNewton(const Operator& F)
  {
    using namespace Newton;
    auto newton = NewtonMethod( F );
    newton.setDampingStrategy<DampingStrategy::AffineContravariant>();
    newton.setTerminationCriterion<TerminationCriterion::AffineContravariant>();
    return newton;
  }
}
