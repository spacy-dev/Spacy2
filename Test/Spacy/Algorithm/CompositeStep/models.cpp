// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/c2Functional.hh"
#include "Spacy/Algorithm/CompositeStep/quadraticModel.hh"
#include "Test/Mock/linearOperator.hh"
#include "Test/mockSetup.hh"

using namespace Spacy;

namespace
{
  struct TestC2Functional
  {
    TestC2Functional(const Spacy::VectorSpace& space)
      : domain_(&space)
    {}

    Spacy::Real operator()(const Spacy::Vector& x) const
    {
      return Real(toDouble(x)*toDouble(x));
    }

    Spacy::Vector d1(const Spacy::Vector& x) const
    {
      return 2*x;
    }

    Spacy::Vector d2(const Spacy::Vector& x, const Spacy::Vector& dx) const
    {
      return 2*dx;
    }

    LinearOperator hessian(const Spacy::Vector&) const
    {
      return Mock::LinearOperator();
    }

    const Spacy::VectorSpace& domain() const
    {
      return *domain_;
    }

  private:
    const Spacy::VectorSpace* domain_;
  };
}

TEST(CompositeStep,QuadraticModel)
{
  auto X = createMockBanachSpace();
  auto f = TestC2Functional(X);

  auto nu = Real(0.5);
  auto dn = Real(1);
  auto dt = Real(2);
  auto x = Real(1);

  auto q = CompositeStep::makeQuadraticModel(nu,dn,dt,f,x);

  EXPECT_DOUBLE_EQ( q(0)  , f(x+nu*dn)      );
  EXPECT_DOUBLE_EQ( q(nu) , f(x+nu*(dn+dt)) );
  EXPECT_DOUBLE_EQ( q(1)  , f(x+nu*dn+dt)   );
}

TEST(CompositeStep,NormModel)
{
  auto nu = Real(0.5);
  auto dn = Real(1);
  auto dt = Real(-2);

  auto q = CompositeStep::makeQuadraticNormModel(nu,dn,dt);

  auto norm2 = [](const auto& x) { return norm(x)*norm(x); };

  EXPECT_DOUBLE_EQ( q(0)  , norm2(nu*dn)      );
  EXPECT_DOUBLE_EQ( q(nu) , norm2(nu*(dn+dt)) );
  EXPECT_DOUBLE_EQ( q(1)  , norm2(nu*dn+dt)   );
}

TEST(CompositeStep,CubicModel)
{
  auto X = createMockBanachSpace();
  auto f = TestC2Functional(X);

  auto nu = Real(0.5), omega = Real(2);
  auto dn = Real(1);
  auto dt = Real(2);
  auto x = Real(1);

  auto q = CompositeStep::makeCubicModel(nu,dn,dt,f,x,omega);
  auto q0 = CompositeStep::makeQuadraticModel(nu,dn,dt,f,x);
  auto q1 = CompositeStep::makeQuadraticNormModel(nu,dn,dt);

  EXPECT_DOUBLE_EQ( q(0)   , q0(0) + omega/6*pow(q1(0),1.5)   );
  EXPECT_DOUBLE_EQ( q(nu)  , q0(nu) + omega/6*pow(q1(nu),1.5) );
  EXPECT_DOUBLE_EQ( q(1)   , q0(1) + omega/6*pow(q1(1),1.5)   );
}

