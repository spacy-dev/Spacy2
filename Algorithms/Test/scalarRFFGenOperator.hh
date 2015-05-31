#ifndef ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
#define ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH

#include <cmath>

#include "../Interface/abstractC1Operator.hh"
#include "../Interface/abstractC2Operator.hh"
#include "../functionSpace.hh"
#include "../functionSpaceElement.hh"
#include "../FunctionSpaces/RealNumbers/real.hh"

namespace Algorithm
{
//  template <class Function>
//  class ScalarRFFGenOperator : public Operator
//  {
//  public:
//    ScalarRFFGenOperator(const Function& f, const FunctionSpace& space)
//      : Operator(space,space), f_(f)
//    {}

//    ~ScalarRFFGenOperator(){}

//    void update(const FunctionSpaceElement &x) override
//    {
//      f_.update(x.coefficient(0));
//    }

//    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const override
//    {
//      return makeElement<Real>( f_(x.coefficient(0)) , this->getRange().impl() );
//    }

//    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
//    {
//      return makeElement<Real>( f_.template d1<0>(dx.coefficient(0.)) , this->getRange().impl() );
//    }

//  private:
//    mutable Function f_;
//  };

//  template <class Function>
//  auto makeScalarRFFGenOperator(const Function& f, const FunctionSpace& space)
//  {
//    return ScalarRFFGenOperator<Function>(f,space);
//  }

  class TestOperator : public AbstractC1Operator
  {
  public:
    TestOperator(const AbstractBanachSpace& space)
      : AbstractC1Operator(space,space)
    {}

    TestOperator(const FunctionSpace& space)
      : TestOperator(space.impl())
    {}

    ~TestOperator(){}

    std::unique_ptr<AbstractC0Operator> clone() const
    {
      return std::make_unique<TestOperator>(getDomain());
    }
//    void setArgument(const AbstractFunctionSpaceElement &x) override
//    {
//      x_ = &x;
//    }

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override
    {
      x_ = &x;
      return std::make_unique<Real>( exp(x_->coefficient(0))-2 , getRange() );
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement &dx) const override
    {
      return std::make_unique<Real>( exp(x_->coefficient(0))*dx.coefficient(0) , getRange() );
    }

  private:
    mutable const AbstractFunctionSpaceElement* x_;
  };

  class TestOperator2 : public AbstractC2Operator
  {
  public:
    TestOperator2(const AbstractBanachSpace& domain, const AbstractBanachSpace& range)
      : AbstractC2Operator(domain,range)
    {}

    TestOperator2(const FunctionSpace& domain, const FunctionSpace& range)
      : TestOperator2(domain.impl(),range.impl())
    {}

    ~TestOperator2(){}

    std::unique_ptr<AbstractC0Operator> clone() const
    {
      return std::make_unique<TestOperator2>(getDomain(),getRange());
    }
//    void setArgument(const AbstractFunctionSpaceElement &x) override
//    {
//      x_ = &x;
//    }

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override
    {
      x_ = &x;
      auto result = getRange().element();
      result->coefficient(0) = exp(x_->coefficient(0))-2*x_->coefficient(1);
      return std::move(result);
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement &dx) const override
    {
      auto result = getRange().element();
      result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0) - 2*dx.coefficient(1);
      return std::move(result);
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
    {
      auto result = getRange().element();
      result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0)*dy.coefficient(0);
      return std::move(result);
    }

  private:
    mutable const AbstractFunctionSpaceElement* x_;
  };
}

#endif // ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
