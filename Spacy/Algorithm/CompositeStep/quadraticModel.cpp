#include "quadraticModel.hh"

#include <Spacy/Util/invoke.hh>
#include <Spacy/c2Functional.hh>
#include <Spacy/scalarProduct.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

#include <cmath>
#include <tuple>

namespace Spacy
{
  namespace CompositeStep
  {
    namespace
    {
      auto quadraticCoefficients(DampingFactor nu, const Vector& dn, const Vector& dt, const C2Functional &L, const Vector& x)
      {
        auto constant = L(x) + nu*L.d1(x)(dn) + 0.5*nu*nu*L.d2(x,dn)(dn);
        auto linear = L.d1(x)(dt) + nu*L.d2(x,dn)(dt);
        auto quadratic = 0.5*L.d2(x,dt)(dt);
        return std::make_tuple(constant,linear,quadratic);
      }
    }


    Quadratic makeQuadraticModel(DampingFactor nu, const Vector& dn, const Vector& dt, const C2Functional &L, const Vector& x)
    {
      return create<Quadratic>(quadraticCoefficients(nu,dn,dt,L,x));
    }


    Quadratic makeQuadraticNormModel(DampingFactor nu, const Vector& dn, const Vector& dt)
    {
      return Quadratic( nu*nu*dn*dn , 2*nu*dn*dt , dt*dt);
    }

    CubicModel makeCubicModel(DampingFactor nu, const Vector& dn, const Vector& dt,
                              const C2Functional& L, const Vector& x, double omega)
    {
      return CubicModel( makeQuadraticModel(nu,dn,dt,L,x), makeQuadraticNormModel(nu,dn,dt), omega );
    }

    CubicModel::CubicModel(const Quadratic& quadraticModel, const Quadratic& squaredNorm, double omega)
      : quadraticModel_(quadraticModel), squaredNorm_(squaredNorm), omega_(omega)
    {}

    Real CubicModel::operator()(Real t) const
    {
      return quadraticModel_(t) + omega_/6 * pow( squaredNorm_(t), 1.5 );
    }
  }
}
