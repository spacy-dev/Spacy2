#ifndef ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
#define ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  namespace DampingStrategy
  {
    template <class Newton>
    struct AffineCovariant
    {
      AffineCovariant(const Newton& method) : newton(method) {}

      double operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& dx)
      {
        auto trial = x + dx;
        auto ds = newton.FInv_(-newton.F_(trial));

        auto dxNorm = newton.norm_(dx);
        auto theta = newton.norm_(ds)/dxNorm;
        auto nu = std::min(1., newton.thetaAim_/theta);
        while( theta > newton.thetaMax_)
        {
          trial = x + nu*dx;
          ds = newton.FInv_( -newton.F_(trial) + (1-nu) * newton.F_(x) );

          theta = newton.norm_(ds)/(nu*dxNorm);
          nu = std::min(1., newton.thetaAim_*nu/theta);
        }
        if( newton.verbose_ ) std::cout << "nu = " << nu << std::endl;
        return nu;
      }

      const Newton& newton;
    };
  }
}

#endif // ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
