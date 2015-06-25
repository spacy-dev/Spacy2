#include <memory>

#include <gtest/gtest.h>

#include "../../Algorithms/FunctionSpaces/RealNumbers/real.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpace.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "../../Algorithms/Interface/Operator/abstractC2Operator.hh"
#include "../../Algorithms/lagrangeFunctional.hh"
#include "../../Algorithms/Operators/trackingTypeCostFunctional.hh"
#include "../../Algorithms/hilbertSpace.hh"
#include "../../Algorithms/banachSpace.hh"
#include "../../Algorithms/functionSpaceElement.hh"
#include "../../Algorithms/c2Functional.hh"
#include "../../Algorithms/c2Operator.hh"


using namespace Algorithm;

class TestOperator2 : public AbstractC2Operator
{
public:
  TestOperator2(const AbstractBanachSpace& domain, const AbstractBanachSpace& range)
    : AbstractC2Operator(domain,range)
  {}

  TestOperator2(const BanachSpace& domain, const BanachSpace& range)
    : TestOperator2(domain.impl(),range.impl())
  {}

  ~TestOperator2(){}

  TestOperator2* cloneImpl() const
  {
    return new TestOperator2(getDomain(),getRange());
  }

  std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement &x) const override
  {
    auto result = getRange().element();
    result->coefficient(0) = exp(x.coefficient(0))-2*x.coefficient(1);
    return std::move(result);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement &dx) const override
  {
    auto result = getRange().element();
    result->coefficient(0) = exp(x.coefficient(0))*dx.coefficient(0) - 2*dx.coefficient(1);
    return std::move(result);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
  {
    auto result = getRange().element();
    result->coefficient(0) = exp(x.coefficient(0))*dx.coefficient(0)*dy.coefficient(0);
    return std::move(result);
  }
};

TEST(LagrangeFunctionalTest, D0Test)
{
  BanachSpace X( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  auto J = makeC2Functional<TrackingTypeCostFunctional>(alpha,referenceState,Y);
  auto c = makeC2Operator<TestOperator2>(Y,P);
  LagrangeFunctional L(J,c);

  auto x = X.element();
  auto y = X.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  y.coefficient(2) = 1;
  auto z = y;
  z.coefficient(2) = 3;
  z.coefficient(0) = 2;
  EXPECT_DOUBLE_EQ( L(x) , 0.5 );
  EXPECT_DOUBLE_EQ( L(primal(x)) , 0.5 );
  EXPECT_DOUBLE_EQ( L(y) , -0.5 + exp(1) );
  EXPECT_DOUBLE_EQ( L(primal(y)) , 1.5 );
  EXPECT_DOUBLE_EQ( L(z) , -4. + 3*exp(2) );
  EXPECT_DOUBLE_EQ( L(primal(z)) , 2. );
}

TEST(LagrangeFunctionalTest, D1Test)
{
  BanachSpace X( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  auto J = makeC2Functional<TrackingTypeCostFunctional>(alpha,referenceState,Y);
  auto c = makeC2Operator<TestOperator2>(Y,P);
  LagrangeFunctional L(J,c);

  auto x = X.element();
  auto y = X.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  y.coefficient(2) = 1;
  auto z = y;
  z.coefficient(2) = 3;
  z.coefficient(0) = 2;
  EXPECT_DOUBLE_EQ( L.d1(x,y) , 0. );
  EXPECT_DOUBLE_EQ( L.d1(x,primal(y)) , -1. );
  EXPECT_DOUBLE_EQ( L.d1(x,dual(y)) , 1. );
  EXPECT_DOUBLE_EQ( L.d1(x,z) , 1. );
  EXPECT_DOUBLE_EQ( L.d1(x,primal(z)) , -2. );
  EXPECT_DOUBLE_EQ( L.d1(x,dual(z)) , 3. );

  EXPECT_DOUBLE_EQ( L.d1(y,y) , 2*exp(1)-1 );
  EXPECT_DOUBLE_EQ( L.d1(y,primal(y)) , exp(1)+1 );
  EXPECT_DOUBLE_EQ( L.d1(y,dual(y)) , exp(1)-2 );
  EXPECT_DOUBLE_EQ( L.d1(y,z) , 5*exp(1)-5 );
  EXPECT_DOUBLE_EQ( L.d1(y,primal(z)) , 2*exp(1)+1 );
  EXPECT_DOUBLE_EQ( L.d1(y,dual(z)) , 3*exp(1)-6 );
}

TEST(LagrangeFunctionalTest, D2Test)
{
  BanachSpace X( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  auto J = makeC2Functional<TrackingTypeCostFunctional>(alpha,referenceState,Y);
  auto c = makeC2Operator<TestOperator2>(Y,P);
  LagrangeFunctional L(J,c);

  auto x = X.element();
  auto y = X.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  y.coefficient(2) = 1;
  auto y2 = y;
  auto z = y;
  z.coefficient(2) = 3;
  z.coefficient(0) = 2;
  EXPECT_DOUBLE_EQ( L.d2(x,primal(y),primal(y)) , 4. );
  EXPECT_DOUBLE_EQ( L.d2(x,primal(y),y2) , 3. );
  EXPECT_DOUBLE_EQ( L.d2(x,y2,primal(y)) , 3. );
  EXPECT_DOUBLE_EQ( L.d2(x,y,y) , 2. );
  EXPECT_DOUBLE_EQ( L.d2(x,dual(y),dual(y2)) , 0. );
  EXPECT_DOUBLE_EQ( L.d2(x,dual(y),y2) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(x,y2,dual(y)) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(x,dual(y),primal(y2)) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(x,primal(y),dual(y2)) , -1. );
}
