#include "quadraticModel.hh"

#include "../../functionSpaceElement.hh"
#include "../../scalarProduct.hh"
#include "../../lagrangeFunctional.hh"
#include "../../FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"

#include <cmath>

namespace Algorithm
{
  namespace CompositeStep
  {
    QuadraticModel::QuadraticModel(double constant, double linear, double quadratic)
      : constant_(constant), linear_(linear), quadratic_(quadratic)
    {}

    double QuadraticModel::operator()(double t) const
    {
      return constant_ + linear_*t + quadratic_*t*t;
    }


    QuadraticModel makeQuadraticModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const LagrangeFunctional &L)
    {
      auto constant = nu*L.d1(primal(dn)) + 0.5*nu*nu*L.d2(primal(dn),primal(dn));
      auto linear = L.d1(primal(dt)) + nu*L.d2(dn,dt);
      auto quadratic = 0.5*L.d2(primal(dt),primal(dt));

      return QuadraticModel( constant, linear, quadratic );
    }


    QuadraticModel makeQuadraticNormModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp)
    {
      return QuadraticModel( sp(primal(dn),primal(dn))*nu*nu , sp(primal(dn),primal(dt))*2*nu , sp(primal(dt),primal(dt)) );
    }

    CubicModel makeCubicModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp, const LagrangeFunctional& f, double omega)
    {
      return CubicModel( makeQuadraticModel(nu,dn,dt,f), makeQuadraticNormModel(nu,dn,dt,sp), omega );
    }

    CubicModel::CubicModel(const QuadraticModel& quadraticModel, const QuadraticModel& squaredNorm, double omega)
      : quadraticModel_(quadraticModel), squaredNorm_(squaredNorm), omega_(omega)
    {}

    double CubicModel::operator()(double t) const
    {
      return quadraticModel_(t) + omega_/6 * pow( squaredNorm_(t), 1.5 );
    }


    LipschitzConstant& LipschitzConstant::operator=(double newOmega)
    {
      omegaOld = omega;
      if( newOmega < 0 ) omega = 0.1*omegaOld;

      omega = std::max(omega,omegaMin);
      omega = std::min(omega,omegaOld*maxFactor);
      return *this;
    }

    LipschitzConstant::operator double() const
    {
      return omega;
    }
  }
}
