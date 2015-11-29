// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "quadraticModel.hh"

#include "Spacy/Util/invoke.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/scalarProduct.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"

#include <cmath>
#include <tuple>

namespace Spacy
{
  namespace CompositeStep
  {
    namespace
    {
      auto quadraticCoefficients(Real nu, const Vector& dn, const Vector& dt, const C2Functional &L, const Vector& x)
      {
        auto constant = L(x) + nu*L.d1(x)(dn) + 0.5*nu*nu*L.d2(x,dn)(dn);
        auto linear = L.d1(x)(dt) + nu*L.d2(x,dn)(dt);
        auto quadratic = 0.5*L.d2(x,dt)(dt);
        return std::make_tuple(constant,linear,quadratic);
      }
    }


    Functions_1D::Quadratic makeQuadraticModel(Real nu, const Vector& dn, const Vector& dt, const C2Functional &L, const Vector& x)
    {
      return create<Functions_1D::Quadratic>(quadraticCoefficients(nu,dn,dt,L,x));
    }


    Functions_1D::Quadratic makeQuadraticNormModel(Real nu, const Vector& dn, const Vector& dt)
    {
      return Functions_1D::Quadratic( nu*nu*dn*dn , 2*nu*dn*dt , dt*dt);
    }

    CubicModel makeCubicModel(Real nu, const Vector& dn, const Vector& dt,
                              const C2Functional& L, const Vector& x, Real omega)
    {
      return CubicModel( makeQuadraticModel(nu,dn,dt,L,x), makeQuadraticNormModel(nu,dn,dt), omega );
    }

    CubicModel::CubicModel(const Functions_1D::Quadratic& quadraticModel, const Functions_1D::Quadratic& squaredNorm, Real omega)
      : quadraticModel_(quadraticModel), squaredNorm_(squaredNorm), omega_(omega)
    {}

    Real CubicModel::operator()(Real t) const
    {
      return quadraticModel_(t) + omega_/6 * pow( squaredNorm_(t), 1.5 );
    }
  }
}
