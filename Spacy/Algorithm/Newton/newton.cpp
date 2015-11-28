// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "newton.hh"

#include "Spacy/derivative.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Util/Exceptions/notConvergedException.hh"

#include <iostream>

namespace Spacy
{
  namespace Newton
  {
    Vector newton(const C1Operator& F, const Vector& x0,
                  const std::function<DampingFactor(const std::function<Vector(const Vector&)>&,const Vector&,const Vector&)>& dampingStrategy,
                  const std::function<bool(DampingFactor,const Vector&,const Vector&)>& terminationCriterion,
                  const Parameter p)
    {
      using namespace std::chrono;
      if( p.verbose() ) std::cout << "Starting newton iteration." << std::endl;
      p.startTimer();

      auto x = x0;
      if( terminationCriterion(1.,x,x) )
        return x;

      for(unsigned i = 1; i <= p.maxSteps(); ++i)
      {
//        if( p.verbose() )
          std::cout << "\nIteration " << i << ": ";

        auto dF_inv = d1(F,x)^-1;

        auto dx = dF_inv(-F(x));
        auto nu = dampingStrategy(dF_inv,x,dx);
        x += nu()*dx;

        if( p.verbose() ) std::cout << "nu = " << nu() << ", |x| = " << norm(x) << ", |dx| = " << norm(dx) << std::endl;

        if( terminationCriterion(nu(),x,dx) )
        {
          if( p.verbose() ) std::cout << "Newton iteration converged. Computation time: " << p.elapsedTime() << "s." << std::endl;
          return x;
        }
      }

      throw Exception::NotConverged("Newton");
    }
  }


  Vector localNewton(const C1Operator& F, const Vector& x0, const Spacy::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::None,Newton::Termination::AffineCovariant>(F,x0,p);
  }

  Vector localNewton(const C1Operator& F, const Newton::Parameter p)
  {
    return localNewton(F,F.domain().zeroVector(),p);
  }


  Vector covariantNewton(const C1Operator& F, const Vector& x0, const Spacy::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::AffineCovariant,Newton::Termination::AffineCovariant>(F,x0,p);
  }

  Vector covariantNewton(const C1Operator& F, const Spacy::Newton::Parameter p)
  {
    return covariantNewton(F,F.domain().zeroVector(),p);
  }


  Vector contravariantNewton(const C1Operator& F, const Vector& x0, const Spacy::Newton::Parameter p)
  {
    return Newton::newton<Newton::Damping::AffineContravariant,Newton::Termination::AffineContravariant>(F,x0,p);
  }

  Vector contravariantNewton(const C1Operator& F, const Spacy::Newton::Parameter p)
  {
    return contravariantNewton(F,F.domain().zeroVector(),p);
  }
}
