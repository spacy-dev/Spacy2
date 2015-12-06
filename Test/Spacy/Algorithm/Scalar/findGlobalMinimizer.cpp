// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Algorithm/Scalar/findGlobalMinimizer.hh"

using namespace Spacy;

TEST(FindGlobalMinimizer,ScalarQuadraticInside)
{
  double a = -10, b = 10, eps = 1e-2;
  auto f = [](Real t) { return (t-1)*(t-1); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( t0 , 1 , (b-a)*eps );
}

TEST(FindGlobalMinimizer,ScalarQuadraticOnBoundary)
{
  double a = -1, b = 2, eps = 1e-2;
  auto f = [](Real t) { return -(t-1)*(t-1); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( t0 , a , (b-a)*eps );
}

TEST(FindGlobalMinimizer,DiscontinuousFourthOrderTwoMinima)
{
  double a = -2, b = 2, eps = 1e-2;

  auto f = [](Real t) { return (t-1)*(t-1)*(t+1)*(t+1) - ( t>0 ? 1 : 0 ); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( t0 , 1 , (b-a)*eps );


  auto g = [](Real t) { return (t-1)*(t-1)*(t+1)*(t+1) + ( t>0 ? 1 : 0 ); };

  t0 = findGlobalMinimizer(g,a,b,eps);
  ASSERT_NEAR( t0 , -1 , (b-a)*eps );
}
