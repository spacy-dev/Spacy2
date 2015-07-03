#include "newton.hh"

#include "linearOperator.hh"
#include "functionSpaceElement.hh"
#include "c1Operator.hh"

#include "Util/Exceptions/regularityTestFailedException.hh"

#include "dampingStrategies.hh"
#include "Algorithm/terminationCriteria.hh"

#include <chrono>
#include <iostream>

namespace Algorithm
{
  Newton::Newton(const C1Operator& F)
    : F_(F),
      dampingFactor_(Newton_DampingStrategy::AffineCovariant(*this,F_,norm_)),
      terminationCriterion_( AffineCovariantRelativeTerminationCriterion(F_,norm_,relativeAccuracy(),verbose()) )
  {}

  FunctionSpaceElement Newton::solve() const
  {
    return solve( FunctionSpaceElement( F_.impl().getDomain().element() ) );
  }

  FunctionSpaceElement Newton::solve(const FunctionSpaceElement& x0) const
  {
    using namespace std::chrono;
    if( verbose() ) std::cout << "Starting newton iteration." << std::endl;
    auto startTime = high_resolution_clock::now();
    norm_ = Norm( x0.impl().getSpace().getSharedNorm() );

    auto x = x0;
    for(unsigned i = 1; i <= maxSteps(); ++i)
    {
      if( verbose() ) std::cout << "Iteration " << i << ": ";

      auto DF = F_.getLinearization(x);

      auto dx = (DF^-1)(-F_(x));
      auto nu = dampingFactor_(DF^-1,x,dx);
      x += nu*dx;

      if( !regularityTestPassed(nu)) throw RegularityTestFailedException("Newton",nu);

      if( verbose() ) std::cout << "nu = " << nu << ", |x| = " << norm_(x) << ", |dx| = " << norm_(dx) << std::endl;

      if( terminationCriterion_(nu,x,dx))
      {
        if( verbose() ) std::cout << "Newton iteration converged. Computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s." << std::endl;
        return x;
      }
    }

    return x;
  }


  Newton localNewton(const C1Operator& F)
  {
    auto newton = Newton( F );
    newton.setDampingStrategy<Newton_DampingStrategy::Undamped>();
    return newton;
  }

  Newton covariantNewton(const C1Operator& F)
  {
    return Newton( F );
  }

  Newton contravariantNewton(const C1Operator& F)
  {
    auto newton = Newton( F );
    newton.setDampingStrategy<Newton_DampingStrategy::AffineContravariant>();
    newton.setTerminationCriterion<AffineContravariantRelativeTerminationCriterion>();
    return newton;
  }
}
