#include "quadraticModel.hh"

#include "vector.hh"
#include "functional.hh"
#include "Util/invoke.hh"

#include <cmath>
#include <tuple>

namespace Algorithm
{
  namespace CompositeStep
  {
    namespace
    {
      auto quadraticCoefficients(double nu, const Vector& dn, const Vector& dt, const Functional &L, const Vector& x)
      {
        auto constant = L(x) + nu*L.d1(x,dn) + 0.5*nu*nu*L.d2(x,dn,dn);
        auto linear = L.d1(x,dt) + nu*L.d2(x,dn,dt);
        auto quadratic = 0.5*L.d2(x,dt,dt);
        return std::make_tuple(constant,linear,quadratic);
      }
    }


    Functions_1D::Quadratic makeQuadraticModel(double nu, const Vector& dn, const Vector& dt, const Functional &L, const Vector& x)
    {
      return create<Functions_1D::Quadratic>(quadraticCoefficients(nu,dn,dt,L,x));
    }


    Functions_1D::Quadratic makeQuadraticNormModel(double nu, const Vector& dn, const Vector& dt)
    {
      return Functions_1D::Quadratic( nu*nu*dn*dn , 2*nu*dn*dt , dt*dt);
    }

    CubicModel makeCubicModel(double nu, const Vector& dn, const Vector& dt,
                              const Functional& f, const Vector& x, double omega)
    {
      return CubicModel( makeQuadraticModel(nu,dn,dt,f,x), makeQuadraticNormModel(nu,dn,dt), omega );
    }

    CubicModel::CubicModel(const Functions_1D::Quadratic& quadraticModel, const Functions_1D::Quadratic& squaredNorm, double omega)
      : quadraticModel_(quadraticModel), squaredNorm_(squaredNorm), omega_(omega)
    {}

    double CubicModel::operator()(double t) const
    {
      return quadraticModel_(t) + omega_/6 * pow( squaredNorm_(t), 1.5 );
    }
  }
}
