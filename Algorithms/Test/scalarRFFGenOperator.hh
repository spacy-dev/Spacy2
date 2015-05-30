#ifndef ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
#define ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH

#include <cmath>

#include "../Interface/abstractDifferentiableOperator.hh"
#include "../Interface/abstractTwiceDifferentiableOperator.hh"
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

  class TestOperator : public AbstractDifferentiableOperator
  {
  public:
    TestOperator(const FunctionSpace& space)
      : space_(space.impl())
    {}

    TestOperator(const AbstractBanachSpace& space)
      : space_(space)
    {}

    ~TestOperator(){}

    std::unique_ptr<AbstractOperator> clone() const
    {
      return std::make_unique<TestOperator>(space_);
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

    const AbstractBanachSpace& getDomain() const override
    {
      return space_;
    }

    const AbstractBanachSpace& getRange() const override
    {
      return space_;
    }

  private:
    mutable const AbstractFunctionSpaceElement* x_;
    const AbstractBanachSpace& space_;
  };

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
}

#endif // ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
