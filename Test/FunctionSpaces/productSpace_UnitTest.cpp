#include <gtest/gtest.h>

#include "../../Algorithms/functionSpace.hh"
#include "../../Algorithms/norm.hh"
#include "../../Algorithms/scalarProduct.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpace.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"

TEST(PrimalDualProductSpaceTest,PurePrimalElementTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> >() );

  auto x = R2.element();
  EXPECT_EQ( dynamic_cast<const PrimalDualProductSpaceElement*>(&x.impl()) == nullptr , false );
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 0. );
}

TEST(PrimalDualProductSpaceTest,PureDualElementTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<> , PackSpaces<RealSpace,RealSpace> >() );

  auto x = R2.element();
  EXPECT_EQ( dynamic_cast<const PrimalDualProductSpaceElement*>(&x.impl()) == nullptr , false );
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 0. );
}

TEST(PrimalDualProductSpaceTest,MixedElementTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );

  auto x = R2.element();
  EXPECT_EQ( dynamic_cast<const PrimalDualProductSpaceElement*>(&x.impl()) == nullptr , false );
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 0. );
}

TEST(PrimalDualProductSpaceTest,PurePrimalElementSumTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 2;

  auto z = x + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );
  z = y + dual(x);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );

  x += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
}

TEST(PrimalDualProductSpaceTest,PureDualElementSumTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<> , PackSpaces<RealSpace,RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 2;

  auto z = x + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );

  x += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 2. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
}

TEST(PrimalDualProductSpaceTest,MixedElementSumTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 2;

  auto z = x + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = x + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );
  z = primal(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = primal(x) + primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );

  z = x + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 1. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );
  z = dual(x) + dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 2. );

  x += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 2. );
  x += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 2. );
  x += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );

  primal(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 4. );
  dual(x) += y;
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 4. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  primal(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  primal(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  dual(x) += primal(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 6. );
  dual(x) += dual(y);
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 5. );
  EXPECT_DOUBLE_EQ( x.coefficient(1) , 8. );
}

TEST(PrimalDualProductSpaceTest,PurePrimalElementProductTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(1) = x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 4.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 4.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 4.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 0.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 0.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 0.);
}

TEST(PrimalDualProductSpaceTest,PureDualElementProductTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<> , PackSpaces<RealSpace,RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(1) = x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 0.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 0.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 0.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 4.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 4.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 4.);
}

TEST(PrimalDualProductSpaceTest,MixedElementProductTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(1) = x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 3;

  EXPECT_DOUBLE_EQ( x*y , 4. );
  EXPECT_DOUBLE_EQ( primal(x)*y , 1.);
  EXPECT_DOUBLE_EQ( x*primal(y) , 1.);
  EXPECT_DOUBLE_EQ( primal(x)*primal(y) , 1.);
  EXPECT_DOUBLE_EQ( dual(x)*y , 3.);
  EXPECT_DOUBLE_EQ( x*dual(y) , 3.);
  EXPECT_DOUBLE_EQ( dual(x)*dual(y) , 3.);
}

TEST(PrimalDualProductSpaceTest,MixedElementArithmeticProductTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(1) = x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 3;

  auto z = 2*y;
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 6. );

  z = 2*primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 2. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );

  z = 2*dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 6. );

  primal(z) = 3*primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 3. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 6. );

  primal(z) = 3*dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 6. );

  dual(z) = 3*primal(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 0. );

  dual(z) = 3*dual(y);
  EXPECT_DOUBLE_EQ( z.coefficient(0) , 0. );
  EXPECT_DOUBLE_EQ( z.coefficient(1) , 9. );
}

TEST(PrimalDualProductSpaceTest,ScalarProductTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(0) = y.coefficient(0) = 1;
  y.coefficient(1) = 2;
  EXPECT_DOUBLE_EQ( x*y, 1. );
  EXPECT_DOUBLE_EQ( x*y, R2.getScalarProduct()(x,y) );
}

TEST(PrimalDualProductSpaceTest,NormTest)
{
  using namespace Algorithm;
  auto R2 = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace> , PackSpaces<RealSpace> >() );
  auto x = R2.element();
  auto y = R2.element();
  x.coefficient(0) = y.coefficient(0) = 3;
  y.coefficient(1) = 4;
  EXPECT_DOUBLE_EQ( R2.getNorm()(x) , 3. );
  EXPECT_DOUBLE_EQ( R2.getNorm()(y) , 5. );
}

