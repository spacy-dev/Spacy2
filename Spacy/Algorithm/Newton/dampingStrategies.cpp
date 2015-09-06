#include "dampingStrategies.hh"

#include "Spacy/Util/Exceptions/regularityTestFailedException.hh"

#include <iostream>

namespace Spacy
{
  namespace Newton
  {
    namespace Damping
    {
      AffineCovariant::AffineCovariant(const Operator& F)
        : F_(F), oldDs(F.domain().vector())
      {}

      DampingFactor AffineCovariant::operator()(const LinearSolver& DFInv_, const Vector& x, const Vector& dx) const
      {
        DampingFactor nu = 1;
        auto mu = Real{1.}, normDx =  norm(dx);

        if( normDx < sqrtEps() * norm(x) ) return nu;

        if( oldNu > 0 )
        {
          mu = normOldDx * normOldDs * oldNu() / ( normDx * norm(oldDs - dx) );
          nu = min(1.,mu);
        }

        while(true)
        {
          if( regularityTestFailed(nu()) ) throw RegularityTestFailedException("Newton::DampingStrategy::AffineCovariant",toDouble(nu()));

          auto trial = x + nu()*dx;
          auto ds = DFInv_(-F_(trial)) - (1.-nu())*dx;
          auto normDs = norm(ds);

          auto muPrime = 0.5 * nu() * nu() / normDs;

          if( normDs/normDx >= 1)
          {
            nu = min(0.5*nu(),muPrime);
            continue;
          }

          auto nuPrime = min(1.,muPrime);

          if( nu == 1 && nuPrime == 1 && normDs < eps() ) break;

          if( nuPrime >= 4*nu())
          {
            nu = 4*nu();
//            nu = nuPrime;
            continue;
          }

          break;
        }

        return nu;
      }


      AffineContravariant::AffineContravariant(const Operator& F)
        : F_(F)
      {}

      DampingFactor AffineContravariant::operator()(const LinearSolver&, const Vector& x, const Vector& dx) const
      {
        DampingFactor nu = 1.;
        auto norm_F_x = norm(F_(x));
        if( norm_F_x < sqrtEps() ) return nu;
        if( muPrime > 0 )
          nu = min( 1. , muPrime*norm_F_x_old/norm_F_x );

        while( true )
        {
          if( !regularityTestPassed(nu())) throw RegularityTestFailedException("Newton::DampingStrategy::AffineContravariant",toDouble(nu()));

          auto trial = x + nu()*dx;

          auto norm_F_trial = norm(F_(trial));

          auto theta = norm_F_trial/norm_F_x;
          muPrime = 0.5*norm_F_x*nu()*nu() / norm( F_(trial) - (1-nu)*F_(x) );

          if( theta >= 1 )
          {
            nu = min( muPrime , 0.5*nu() );
            continue;
          }

          auto oldNu = nu;
          nu = min( 1. , muPrime );
          if( nu >= 4*oldNu() )
          {
            nu = 4*oldNu();
            continue;
          }

          norm_F_x_old = norm_F_x;
          break;
        }

        return nu;
      }


      None::None(const Operator&)
      {}

      DampingFactor None::operator()(const LinearSolver&, const Vector&, const Vector&) const
      {
        return 1;
      }
    }
  }
}
