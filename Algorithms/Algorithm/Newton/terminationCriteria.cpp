#include "terminationCriteria.hh"

#include "vector.hh"
#include "operator.hh"

#include <cmath>

namespace Algorithm
{
  namespace Newton
  {
    namespace TerminationCriterion
    {
      Base::Base(double relativeAccuracy, bool verbose)
        : Mixin::RelativeAccuracy(relativeAccuracy),
          Mixin::Verbosity(verbose)
      {}

      AffineCovariant::AffineCovariant(const Operator&, double relativeAccuracy, bool verbose)
        : Base(relativeAccuracy,verbose)
      {}

      bool AffineCovariant::passed(double nu, const Vector& x, const Vector& dx) const
      {
        if(std::fabs(nu-1) > eps()) return false;

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
        : Base(relativeAccuracy,verbose), F_(F)
      {}

      bool AffineContravariant::passed(double nu, const Vector& x, const Vector&) const
      {
        if( initialResidual < 0 )
        {
          initialResidual = norm( F_(x) );
          if( verbose() ) std::cout << "Initial residual: " << initialResidual << std::endl;
          return false;
        }

        if(std::fabs(nu-1) > eps()) return false;

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

