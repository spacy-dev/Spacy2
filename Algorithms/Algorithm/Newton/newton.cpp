#include "newton.hh"

#include "linearOperator.hh"
#include "functionSpaceElement.hh"
#include "c1Operator.hh"

#include "Util/regularityTestFailedException.hh"

#include <iostream>

namespace Algorithm
{
  Newton::Newton(const C1Operator& F)
    : F_(F),
      dampingFactor_(Newton_DampingStrategy::AffineCovariant(*this,F_,norm_))
  {}

  FunctionSpaceElement Newton::solve() const
  {
    return solve( FunctionSpaceElement( F_.impl().getDomain().element() ) );
  }

  FunctionSpaceElement Newton::solve(const FunctionSpaceElement& x0) const
  {
    if( isVerbose() ) std::cout << "Starting newton iteration with initial guess: " << x0;

    norm_ = Norm( x0.impl().getSpace().getSharedNorm() );

    auto x = x0;
    for(unsigned i = 1; i <= maxSteps(); ++i)
    {
      if( isVerbose() ) std::cout << "Iteration: " << i << ", ";

      LinearSolver DFInv(F_.getLinearization(x));
      auto dx = DFInv(-F_(x));

      if( norm_(dx) < relativeAccuracy() * norm_(x) )
      {
        if( isVerbose() ) std::cout << "Terminating (rel. acc.: " << norm_(dx)/norm_(x) << ")\n";
        return x;
      }

      auto nu = dampingFactor_(DFInv,x,dx);
      if( !regularityTestPassed(nu)) throw RegularityTestFailedException("Newton",nu);
      x += nu*dx;
      if( isVerbose() ) std::cout << "nu = " << nu << ", |dx| = " << norm_(dx) << ", |x| = " << norm_(x) << std::endl;
    }

    return x;
  }


  Newton localNewton(const C1Operator& F)
  {
    auto newton = Newton( F );
    newton.setDampingStrategy<Newton_DampingStrategy::Undamped>();
    return newton;
  }

  Newton affineCovariantNewton(const C1Operator& F)
  {
    return Newton( F );
  }

  Newton affineContravariantNewton(const C1Operator& F)
  {
    auto newton = Newton( F );
    newton.setDampingStrategy<Newton_DampingStrategy::AffineContravariant>();
    return newton;
  }
}
