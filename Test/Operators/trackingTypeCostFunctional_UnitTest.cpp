#include <gtest/gtest.h>

#include "../../Algorithms/Operators/trackingTypeCostFunctional.hh"
#include "../../Algorithms/functionSpace.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "../../Algorithms/c2Functional.hh"

TEST(TrackingTypeCostFunctional,D0Test)
{
  using namespace Algorithm;
  auto R = FunctionSpace( std::make_unique<RealSpace>() );
  auto R2 = FunctionSpace( makeProductSpace< PackSpaces<RealSpace,RealSpace> >() );
  auto reference = R2.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  C2Functional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,R2,R));

  auto x = R2.element();
  auto y = R2.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  EXPECT_DOUBLE_EQ( J(x) , 0.5 );
  EXPECT_DOUBLE_EQ( J(y) , 1.5 );
}

TEST(TrackingTypeCostFunctional,D1Test)
{
  using namespace Algorithm;
  auto R = FunctionSpace( std::make_unique<RealSpace>() );
  auto R2 = FunctionSpace( makeProductSpace< PackSpaces<RealSpace,RealSpace> >() );
  auto reference = R2.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  C2Functional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,R2,R));

  auto x = R2.element();
  auto y = R2.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  J(x);
  EXPECT_DOUBLE_EQ( J.d1(y) , -1. );
  J(y);
  EXPECT_DOUBLE_EQ( J.d1(y) , 3. );
}

TEST(TrackingTypeCostFunctional,D2Test)
{
  using namespace Algorithm;
  auto R = FunctionSpace( std::make_unique<RealSpace>() );
  auto R2 = FunctionSpace( makeProductSpace< PackSpaces<RealSpace,RealSpace> >() );
  auto reference = R2.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  C2Functional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,R2,R));

  auto x = R2.element();
  auto y = R2.element();
  auto z = R2.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  z.coefficient(0) = 1;
  z.coefficient(1) = 2;
  J(x);
  EXPECT_DOUBLE_EQ( J.d2(y,z) , 7. );
  J(y);
  EXPECT_DOUBLE_EQ( J.d2(y,z) , 7. );
  J(x);
  EXPECT_DOUBLE_EQ( J.d2(z,z) , 13. );
  J(y);
  EXPECT_DOUBLE_EQ( J.d2(z,z) , 13. );
}
