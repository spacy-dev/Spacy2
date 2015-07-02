#include "dampingStrategies.hh"

#include "functionSpaceElement.hh"
#include "c1Operator.hh"
#include "linearSolver.hh"
#include "newton.hh"

namespace Algorithm
{
  namespace Newton_DampingStrategy
  {
    AffineCovariant::AffineCovariant(const NewtonParameter& p, const C1Operator& F, const Norm& norm)
      : p_(p), F_(F), norm_(norm)
    {}

    double AffineCovariant::operator()(const LinearSolver& DFInv_, const FunctionSpaceElement& x, const FunctionSpaceElement& dx)
    {
      auto trial = x + dx;
      auto ds = DFInv_(-F_(trial));

      std::cout << "0 ds: " << norm_(ds) << ", dx: " << norm_(dx) << ", x: " << norm_(x) << ", trial: " << norm_(trial) << std::endl;

      auto dxNorm = norm_(dx);
      auto theta = norm_(ds)/dxNorm;
      auto nu = std::min(1., p_.thetaAim()/theta);
      while( theta > p_.thetaMax())
      {
        if( !p_.regularityTestPassed(nu) ) break;

        trial = x + nu*dx;
        ds = DFInv_( -F_(trial) + (1-nu) * F_(x) );
        theta = norm_(ds)/(nu*dxNorm);
        std::cout << "nu: " << nu << ", theta: " << theta << std::endl;
        std::cout << "0 ds: " << norm_(ds) << ", dx: " << norm_(dx) << ", x: " << norm_(x) << ", trial: " << norm_(trial) << std::endl;

        nu = std::min(1., p_.thetaAim()*nu/theta);
      }

      return nu;
    }


    AffineContravariant::AffineContravariant(const NewtonParameter& p, const C1Operator& F, const Norm& norm)
      : p_(p), F_(F), norm_(norm)
    {}

    double AffineContravariant::operator()(const LinearSolver&, const FunctionSpaceElement& x, const FunctionSpaceElement& dx)
    {
      auto nu = 1.;
      auto norm_F_x = norm_(F_(x));
      if( muPrime > 0 )
        nu = std::min( 1. , muPrime*norm_F_x_old/norm_F_x );

      while( true )
      {
        if( !p_.regularityTestPassed(nu) ) break;

        auto trial = x + nu*dx;

        auto norm_F_trial = norm_(F_(trial));

        auto theta = norm_F_trial/norm_F_x;
        muPrime = 0.5*norm_F_x*nu*nu / norm_( F_(trial) - (1-nu)*F_(x) );

        if( theta >= 1 )
        {
          nu = std::min( muPrime , 0.5*nu );
          norm_F_x_old = norm_F_x;
          continue;
        }

        auto oldNu = nu;
        nu = std::min( 1. , muPrime );
        if( nu >= 4*oldNu ) continue;
        break;
      }

      return nu;
    }


    Undamped::Undamped(const NewtonParameter&, const C1Operator& F, const Norm& norm)
    {}

    double Undamped::operator()(const LinearSolver&, const FunctionSpaceElement&, const FunctionSpaceElement&)
    {
      return 1;
    }
  }
}
