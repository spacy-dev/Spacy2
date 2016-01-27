// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "genericRothe.hh"

#include "Spacy/dynamicOperator.hh"
#include "Spacy/vector.hh"
#include "Spacy/linearOperator.hh"

#include <iostream>
#include <utility>

namespace Spacy
{
  namespace Rothe
  {
    class TimeDependentAxpy
    {
    public:
      TimeDependentAxpy(LinearOperator M, LinearOperator J)
        : M_(std::move(M)), J_(std::move(J))
      {}

      LinearOperator operator()(double dt) const
      {
        auto Axpy = J_;
        Axpy *= -dt;
        Axpy += M_;
        return Axpy;
//        return axpy(M_,-dt,J_);
      }

      LinearOperator M_, J_;
    };

    Vector genericMethod(const DynamicC1Operator& A, double t0, double t1)
    {
      unsigned maxSteps = 100;

      auto t = t0,
          dt = (t1-t0)/maxSteps;
      auto x = A.domain().zeroVector();

      auto integrator = [&A,&dt](double t, const Vector& x) -> Vector
      {
        return ( ( TimeDependentAxpy(A.linearization(t,x),A.M())(dt) )^-1 )( -A(t,x) );
      };

      for( auto i=0u ; i<maxSteps ; ++i )
      {
        t += dt;
        std::cout << "Iteration " << i << ", t = " << t << std::endl;

        x += integrator(t,x);
      }

      return x;
    }
  }
}
