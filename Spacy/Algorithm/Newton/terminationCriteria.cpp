// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "terminationCriteria.hh"

#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/vector.hh"
#include "Spacy/operator.hh"

#include <cmath>
#include <iostream>

namespace Spacy
{
  namespace Newton
  {
    namespace Termination
    {
//      Base::Base(double relativeAccuracy, bool verbose)
//        : Mixin::RelativeAccuracy(relativeAccuracy),
//          Mixin::Verbosity(verbose)
//      {}

//      bool Base::operator()(double nu, const Vector& x, const Vector& dx) const
//      {
//        return passed(nu,x,dx);
//      }


      AffineCovariant::AffineCovariant(const Operator&, double relativeAccuracy, bool verbose)
        : Mixin::RelativeAccuracy(relativeAccuracy),
          Mixin::Verbosity(verbose)
      {}

      bool AffineCovariant::operator()(DampingFactor nu, const Vector& x, const Vector& dx) const
      {
        if(beforeFirstIteration_)
        {
          beforeFirstIteration_ = false;
          return false;
        }
        if(abs(nu()-1) > eps()) return false;

        auto norm_x = norm(x), norm_dx = norm(dx);
        if( norm_x == 0 && norm_dx == 0 ) return true;

        if( norm_dx < relativeAccuracy() * norm_x )
        {
          if( verbose() ) std::cout << "Terminating (rel. acc.: " << norm_dx/norm_x << ")\n";
          return true;
        }

        return false;
      }


      AffineContravariant::AffineContravariant(const Operator& F, double relativeAccuracy, bool verbose)
        : Mixin::RelativeAccuracy(relativeAccuracy),
          Mixin::Verbosity(verbose), F_(F)
      {}

      bool AffineContravariant::operator()(DampingFactor nu, const Vector& x, const Vector&) const
      {
        if( initialResidual < 0 )
        {
          initialResidual = norm( F_(x) );
          if( verbose() ) std::cout << "Initial residual: " << initialResidual << std::endl;
          if( initialResidual == 0 )
            return true;
          return false;
        }

        if(abs(nu()-1) > eps()) return false;

        if( verbose() ) std::cout << std::scientific << "Residual: " << norm(F_(x)) << ", relative accuracy: " << relativeAccuracy() << ", initialResidual: " << initialResidual << std::endl;

        if( norm( F_(x) ) < relativeAccuracy() * initialResidual )
        {
          if( verbose() ) std::cout << "Terminating (rel. acc.: " << norm(F_(x))/initialResidual << ")\n";
          return true;
        }

        return false;
      }
    }
  }
}

