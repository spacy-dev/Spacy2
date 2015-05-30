#include <memory>

#include <gtest/gtest.h>

#include "../../Algorithms/FunctionSpaces/RealNumbers/real.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpace.hh"
#include "../../Algorithms/FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpace.hh"
#include "../../Algorithms/FunctionSpaces/PrimalDualProductSpace/primalDualProductSpaceElement.hh"
#include "../../Algorithms/Interface/abstractTwiceDifferentiableOperator.hh"
#include "../../Algorithms/lagrangeFunctional.hh"
#include "../../Algorithms/Operators/trackingTypeCostFunctional.hh"


using namespace Algorithm;

class TestOperator2 : public AbstractTwiceDifferentiableOperator
{
public:
  TestOperator2(const FunctionSpace& domain, const FunctionSpace& range)
    : domain_(domain.impl()), range_(range.impl())
  {}

  TestOperator2(const AbstractBanachSpace& domain, const AbstractBanachSpace& range)
    : domain_(domain), range_(range)
  {}

  ~TestOperator2(){}

  std::unique_ptr<AbstractOperator> clone() const
  {
    return std::make_unique<TestOperator2>(domain_,range_);
  }
//    void setArgument(const AbstractFunctionSpaceElement &x) override
//    {
//      x_ = &x;
//    }

  std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override
  {
    x_ = &x;
    auto result = range_.element();
    result->coefficient(0) = exp(x_->coefficient(0))-2*x_->coefficient(1);
    return std::move(result);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement &dx) const override
  {
    auto result = range_.element();
    result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0) - 2*dx.coefficient(1);
    return std::move(result);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
  {
    auto result = range_.element();
    result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0)*dy.coefficient(0);
    return std::move(result);
  }

  const AbstractBanachSpace& getDomain() const override
  {
    return domain_;
  }

  const AbstractBanachSpace& getRange() const override
  {
    return range_;
  }

private:
  mutable const AbstractFunctionSpaceElement* x_;
  const AbstractBanachSpace& domain_;
  const AbstractBanachSpace& range_;
};

TEST(LagrangeFunctionalTest, D0Test)
{
  auto X = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  TwiceDifferentiableFunctional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,Y,P));
  TwiceDifferentiableOperator c(std::make_shared<TestOperator2>(Y,P));
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
  auto X = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  TwiceDifferentiableFunctional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,Y,P));
  TwiceDifferentiableOperator c(std::make_shared<TestOperator2>(Y,P));
  LagrangeFunctional L(J,c);

  auto x = X.element();
  auto y = X.element();
  y.coefficient(0) = 1;
  y.coefficient(1) = 1;
  y.coefficient(2) = 1;
  auto z = y;
  z.coefficient(2) = 3;
  z.coefficient(0) = 2;
  L(x);
  EXPECT_DOUBLE_EQ( L.d1(y) , 0. );
  EXPECT_DOUBLE_EQ( L.d1(primal(y)) , -1. );
  EXPECT_DOUBLE_EQ( L.d1(dual(y)) , 1. );
  EXPECT_DOUBLE_EQ( L.d1(z) , 1. );
  EXPECT_DOUBLE_EQ( L.d1(primal(z)) , -2. );
  EXPECT_DOUBLE_EQ( L.d1(dual(z)) , 3. );
  L(y);
  EXPECT_DOUBLE_EQ( L.d1(y) , 2*exp(1)-1 );
  EXPECT_DOUBLE_EQ( L.d1(primal(y)) , exp(1)+1 );
  EXPECT_DOUBLE_EQ( L.d1(dual(y)) , exp(1)-2 );
  EXPECT_DOUBLE_EQ( L.d1(z) , 5*exp(1)-5 );
  EXPECT_DOUBLE_EQ( L.d1(primal(z)) , 2*exp(1)+1 );
  EXPECT_DOUBLE_EQ( L.d1(dual(z)) , 3*exp(1)-6 );
}

TEST(LagrangeFunctionalTest, D2Test)
{
  auto X = FunctionSpace( makePrimalDualProductSpace< PackSpaces<RealSpace,RealSpace> , PackSpaces<RealSpace> >() );
  const auto& Y = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getPrimalProductSpace();
  const auto& P = dynamic_cast<const PrimalDualProductSpace&>(X.impl()).getDualProductSpace();
  FunctionSpaceElement reference = Y.element();
  reference.coefficient(0) = 1;

  double alpha = 3;
  const auto& referenceState = dynamic_cast<const ProductSpaceElement&>(reference.impl()).variable(0);
  TwiceDifferentiableFunctional J(std::make_shared<TrackingTypeCostFunctional>(alpha,referenceState,Y,P));
  TwiceDifferentiableOperator c(std::make_shared<TestOperator2>(Y,P));
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
  L(x);
  EXPECT_DOUBLE_EQ( L.d2(primal(y),primal(y)) , 4. );
  EXPECT_DOUBLE_EQ( L.d2(primal(y),y2) , 3. );
  EXPECT_DOUBLE_EQ( L.d2(y2,primal(y)) , 3. );
  EXPECT_DOUBLE_EQ( L.d2(y,y) , 2. );
  EXPECT_DOUBLE_EQ( L.d2(dual(y),dual(y2)) , 0. );
  EXPECT_DOUBLE_EQ( L.d2(dual(y),y2) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(y2,dual(y)) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(dual(y),primal(y2)) , -1. );
  EXPECT_DOUBLE_EQ( L.d2(primal(y),dual(y2)) , -1. );
//  L(y);
//  EXPECT_DOUBLE_EQ( L.d1(y) , 2*exp(1)-1 );
//  EXPECT_DOUBLE_EQ( L.d1(primal(y)) , exp(1)+1 );
//  EXPECT_DOUBLE_EQ( L.d1(dual(y)) , exp(1)-2 );
//  EXPECT_DOUBLE_EQ( L.d1(z) , 5*exp(1)-5 );
//  EXPECT_DOUBLE_EQ( L.d1(primal(z)) , 2*exp(1)+1 );
//  EXPECT_DOUBLE_EQ( L.d1(dual(z)) , 3*exp(1)-6 );
}
