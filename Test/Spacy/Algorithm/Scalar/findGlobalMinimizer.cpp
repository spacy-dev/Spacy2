#include <Test/gtest.hh>

#include <Spacy/Algorithm/Scalar/findGlobalMinimizer.hh>

using namespace Spacy;

TEST(FindGlobalMinimizer,ScalarQuadraticInside)
{
  auto a = Real(-10), b = Real(10), eps = Real(1e-2);
  auto f = [](Real t) { return (t-1)*(t-1); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( get(t0) , 1 , get((b-a)*eps) );
}

TEST(FindGlobalMinimizer,ScalarQuadraticOnBoundary)
{
  auto a = Real(-1), b = Real(2), eps = Real(1e-2);
  auto f = [](Real t) { return -(t-1)*(t-1); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( get(t0) , get(a) , get((b-a)*eps) );
}

TEST(FindGlobalMinimizer,DiscontinuousFourthOrderTwoMinima)
{
    auto a = Real(-2), b = Real(2), eps = Real(1e-2);

  auto f = [](Real t) { return (t-1)*(t-1)*(t+1)*(t+1) - ( t>0 ? 1 : 0 ); };

  auto t0 = findGlobalMinimizer(f,a,b,eps);
  ASSERT_NEAR( get(t0) , 1 , get((b-a)*eps) );


  auto g = [](Real t) { return (t-1)*(t-1)*(t+1)*(t+1) + ( t>0 ? 1 : 0 ); };

  t0 = findGlobalMinimizer(g,a,b,eps);
  ASSERT_NEAR( get(t0) , -1 , get((b-a)*eps) );
}
