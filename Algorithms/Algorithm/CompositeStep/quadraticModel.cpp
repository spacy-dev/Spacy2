#include "quadraticModel.hh"

#include "../../functionSpaceElement.hh"
#include "../../scalarProduct.hh"
#include "../../c2Functional.hh"

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


    QuadraticModel makeQuadraticModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const C2Functional &L, const FunctionSpaceElement& x)
    {
      auto constant = L(x) + nu*L.d1(x,dn) + 0.5*nu*nu*L.d2(x,dn,dn);
      auto linear = L.d1(x,dt) + nu*L.d2(x,dn,dt);
      auto quadratic = 0.5*L.d2(x,dt,dt);

      return QuadraticModel( constant, linear, quadratic );
    }


    QuadraticModel makeQuadraticNormModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp)
    {
      return QuadraticModel( sp(dn,dn)*nu*nu , sp(dn,dt)*2*nu , sp(dt,dt) );
    }

    CubicModel makeCubicModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp,
                              const C2Functional& f, const FunctionSpaceElement& x, double omega)
    {
      return CubicModel( makeQuadraticModel(nu,dn,dt,f,x), makeQuadraticNormModel(nu,dn,dt,sp), omega );
    }

    CubicModel::CubicModel(const QuadraticModel& quadraticModel, const QuadraticModel& squaredNorm, double omega)
      : quadraticModel_(quadraticModel), squaredNorm_(squaredNorm), omega_(omega)
    {}

    double CubicModel::operator()(double t) const
    {
      return quadraticModel_(t) + omega_/6 * pow( squaredNorm_(t), 1.5 );
    }
  }
}
