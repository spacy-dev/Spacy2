#include "newton.hh"

#include "linearOperator.hh"
#include "functionSpaceElement.hh"

#include "dampingStrategies.hh"
#include "terminationCriteria.hh"

#include <chrono>
#include <iostream>

namespace Algorithm
{
  namespace Newton
  {
    NewtonMethod::NewtonMethod(const C1Operator& F)
      : F_(F),
        dampingFactor_(DampingStrategy::AffineCovariant(*this,F_,norm_)),
        terminationCriterion_( TerminationCriterion::AffineCovariant(F_,norm_,relativeAccuracy(),true) )
    {}

    FunctionSpaceElement NewtonMethod::solve() const
    {
      return solve( FunctionSpaceElement( F_.impl().domain().element() ) );
    }

    FunctionSpaceElement NewtonMethod::solve(const FunctionSpaceElement& x0) const
    {
      using namespace std::chrono;
      if( verbose() ) std::cout << "Starting newton iteration." << std::endl;
      auto startTime = high_resolution_clock::now();
      norm_ = Norm( x0.impl().space().getSharedNorm() );

      auto x = x0;
      for(unsigned i = 1; i <= maxSteps(); ++i)
      {
        if( verbose() ) std::cout << "Iteration " << i << ": ";

        auto DF_Inv = F_.getLinearization(x)^-1;

        auto dx = DF_Inv(-F_(x));
        auto nu = dampingFactor_(DF_Inv,x,dx);
        x += nu*dx;

        if( verbose() ) std::cout << "nu = " << nu << ", |x| = " << norm_(x) << ", |dx| = " << norm_(dx) << std::endl;

        if( terminationCriterion_(nu,x,dx))
        {
          if( verbose() ) std::cout << "Newton iteration converged. Computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s." << std::endl;
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
