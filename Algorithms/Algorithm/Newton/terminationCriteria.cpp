#include "terminationCriteria.hh"

#include "functionSpaceElement.hh"
#include "norm.hh"
#include "operator.hh"

#include <cmath>

namespace Algorithm
{
  namespace Newton
  {
    namespace TerminationCriterion
    {
      AffineCovariant::AffineCovariant(const Operator&, const Norm& norm, double relativeAccuracy, bool verbose)
        : Mixin::RelativeAccuracy(relativeAccuracy), Mixin::Verbosity(verbose), norm_(norm)
      {}

      bool AffineCovariant::operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const
      {
        if(std::fabs(nu-1) > eps()) return false;

        if( norm_(x) == 0 && norm_(dx) == 0 ) return true;

        if( norm_(dx) < relativeAccuracy() * norm_(x) )
        {
          if( verbose() ) std::cout << "Terminating (rel. acc.: " << norm_(dx)/norm_(x) << ")\n";
          return true;
        }

        return false;
      }


      AffineContravariant::AffineContravariant(const Operator& F, const Norm& norm, double relativeAccuracy, bool verbose)
        : Mixin::RelativeAccuracy(relativeAccuracy), Mixin::Verbosity(verbose), F_(F), norm_(norm)
      {}

      bool AffineContravariant::operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement&) const
      {
        if(std::fabs(nu-1) > eps()) return false;

        if( initialResidual < 0 )
        {
          initialResidual = norm_( F_(x) );
          return false;
        }

        if( norm_( F_(x) ) < relativeAccuracy() * initialResidual ) return true;

        return false;
      }
    }
  }
}

