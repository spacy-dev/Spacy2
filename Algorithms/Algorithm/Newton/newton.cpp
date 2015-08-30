#include "newton.hh"

#include <iostream>

namespace Algorithm
{
  namespace Newton
  {
    Vector newton(const C1Operator& F, const Vector& x0,
                  const DampingStrategy& dampingStrategy,
                  const TerminationCriterion& terminationCriterion,
                  const Newton::Parameter p)
    {
      using namespace std::chrono;
      if( p.verbose() ) std::cout << "Starting newton iteration." << std::endl;
      p.startTimer();

      auto x = x0;
      for(unsigned i = 1; i <= p.maxSteps(); ++i)
      {
        if( p.verbose() ) std::cout << "\nIteration " << i << ": ";

        auto DF_Inv = linearize(F,x)^-1;

        auto dx = DF_Inv(-F(x));
        double nu = dampingStrategy(DF_Inv,x,dx);
        x += nu*dx;

        if( p.verbose() ) std::cout << "nu = " << nu << ", |x| = " << norm(x) << ", |dx| = " << norm(dx) << std::endl;

        if( terminationCriterion(nu,x,dx) )
        {
          if( p.verbose() ) std::cout << "Newton iteration converged. Computation time: " << p.elapsedTime() << "s." << std::endl;
          return x;
        }
      }

      return x;
    }
  }


  Vector localNewton(const C1Operator& F, const Vector& x0, const Algorithm::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::None,Newton::Termination::AffineCovariant>(F,x0,p);
  }

  Vector localNewton(const C1Operator& F, const Newton::Parameter p)
  {
    return localNewton(F,F.domain().vector(),p);
  }


  Vector covariantNewton(const C1Operator& F, const Vector& x0, const Algorithm::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::AffineCovariant,Newton::Termination::AffineCovariant>(F,x0,p);
  }

  Vector covariantNewton(const C1Operator& F, const Algorithm::Newton::Parameter p)
  {
    return covariantNewton(F,F.domain().vector(),p);
  }


  Vector contravariantNewton(const C1Operator& F, const Vector& x0, const Algorithm::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::AffineContravariant,Newton::Termination::AffineContravariant>(F,x0,p);
  }

  Vector contravariantNewton(const C1Operator& F, const Algorithm::Newton::Parameter p)
  {
    return contravariantNewton(F,F.domain().vector(),p);
  }
}
