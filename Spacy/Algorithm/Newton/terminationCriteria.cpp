#include "terminationCriteria.hh"

#include "Spacy/Algorithm/dampingFactor.hh"

#include <cmath>

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
          return false;
        }

        if(abs(nu()-1) > eps()) return false;

        if( verbose() ) std::cout << "Residual: " << norm(F_(x)) << std::endl;

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

