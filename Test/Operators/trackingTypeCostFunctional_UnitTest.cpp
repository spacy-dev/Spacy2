//#include <gtest/gtest.h>

//#include "../../Algorithms/Operators/trackingTypeCostFunctional.hh"
//#include "../../Algorithms/hilbertSpace.hh"
//#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
//#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpace.hh"
//#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpaceElement.hh"
//#include "../../Algorithms/c2Functional.hh"

//TEST(TrackingTypeCostFunctional,D0Test)
//{
//  using namespace Algorithm;
//  auto R2 = makeProductSpace( { std::make_shared<RealSpace>() , std::make_shared<RealSpace>() } );
//  auto reference = R2.element();
//  reference.coefficient(0) = 1;

//  double alpha = 3;
//  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
//  auto J = createFromUniqueImpl<C2Functional,TrackingTypeCostFunctional>(alpha,referenceState,R2);

//  auto x = R2.element();
//  auto y = R2.element();
//  y.coefficient(0) = 1;
//  y.coefficient(1) = 1;
//  EXPECT_DOUBLE_EQ( J(x) , 0.5 );
//  EXPECT_DOUBLE_EQ( J(y) , 1.5 );
//}

//TEST(TrackingTypeCostFunctional,D1Test)
//{
//  using namespace Algorithm;
//  auto R2 = makeProductSpace( { std::make_shared<RealSpace>() , std::make_shared<RealSpace>() } );
//  auto reference = R2.element();
//  reference.coefficient(0) = 1;

//  double alpha = 3;
//  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
//  auto J = createFromUniqueImpl<C2Functional,TrackingTypeCostFunctional>(alpha,referenceState,R2);

//  auto x = R2.element();
//  auto y = R2.element();
//  y.coefficient(0) = 1;
//  y.coefficient(1) = 1;
//  EXPECT_DOUBLE_EQ( J.d1(x,y) , -1. );
//  EXPECT_DOUBLE_EQ( J.d1(y,y) , 3. );
//}

//TEST(TrackingTypeCostFunctional,D2Test)
//{
//  using namespace Algorithm;
//  auto R2 = makeProductSpace( { std::make_shared<RealSpace>() , std::make_shared<RealSpace>() } );
//  auto reference = R2.element();
//  reference.coefficient(0) = 1;

//  double alpha = 3;
//  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
//  auto J = createFromUniqueImpl<C2Functional,TrackingTypeCostFunctional>(alpha,referenceState,R2);

//  auto x = R2.element();
//  auto y = R2.element();
//  auto z = R2.element();
//  y.coefficient(0) = 1;
//  y.coefficient(1) = 1;
//  z.coefficient(0) = 1;
//  z.coefficient(1) = 2;
//  EXPECT_DOUBLE_EQ( J.d2(x,y,z) , 7. );
//  EXPECT_DOUBLE_EQ( J.d2(y,y,z) , 7. );
//  EXPECT_DOUBLE_EQ( J.d2(x,z,z) , 13. );
//  EXPECT_DOUBLE_EQ( J.d2(y,z,z) , 13. );
//}
