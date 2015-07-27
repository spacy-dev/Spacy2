#ifndef ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
#define ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH

#include <cmath>

#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "hilbertSpace.hh"
#include "functionSpaceElement.hh"
#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/RealNumbers/realSolver.hh"

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
//      return makeElement<Real>( f_(x.coefficient(0)) , this->range().impl() );
//    }

//    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
//    {
//      return makeElement<Real>( f_.template d1<0>(dx.coefficient(0.)) , this->range().impl() );
//    }

//  private:
//    mutable Function f_;
//  };

//  template <class Function>
//  auto makeScalarRFFGenOperator(const Function& f, const FunctionSpace& space)
//  {
//    return ScalarRFFGenOperator<Function>(f,space);
//  }

  class TestOperator : public Interface::AbstractC1Operator
  {
  public:
//    TestOperator(const std::shared_ptr<Interface::AbstractBanachSpace> space, const Interface::AbstractFunctionSpaceElement& x)
//      : Interface::AbstractC1Operator(space,space)
//    {
//      setArgument(x);
//      updateSolver();
//    }

    TestOperator(std::shared_ptr<Interface::AbstractBanachSpace> space)
      : Interface::AbstractC1Operator(space,space)
    {}

    TestOperator(const HilbertSpace& space)
      : TestOperator(space.sharedImpl())
    {}

    ~TestOperator(){}

//    void setArgument(const Interface::AbstractFunctionSpaceElement &x)
//    {
//      Interface::AbstractC1Operator::setArgument(x);
//      updateSolver();
//    }

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const override
    {
      return std::make_unique<Real>( exp(x.coefficient(0))-2 , range() );
    }


  private:
    std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement &dx) const override
    {
      return std::make_unique<Real>( exp(x.coefficient(0))*dx.coefficient(0) , range() );
    }

    TestOperator* cloneImpl() const
    {
      return new TestOperator(sharedDomain());
    }

    std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const override
    {
      value = exp(x.coefficient(0));
      return Interface::AbstractC1Operator::makeLinearization(x);
    }

    mutable double value;

    std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
    {
      return std::make_unique<RealSolver>( value , sharedRange() , sharedDomain() );
    }

//    void updateSolver()
//    {
//      solver_ = std::make_shared<RealSolver>(exp(getArgument().coefficient(0)));
//      getLinearization().setSolver(solver_);
//    }

    //std::shared_ptr<AbstractLinearSolver> solver_;

//    virtual std::unique_ptr<AbstractLinearOperator> getLinearization() const
//    {
//      return std::make_unique<RealSolver>(exp(x_->coefficient(0)));
//    }
  };

//  class TestOperator2 : public AbstractC2Operator
//  {
//  public:
//    TestOperator2(const std::shared_ptr<AbstractBanachSpace> domain, const std::shared_ptr<AbstractBanachSpace> range)
//      : AbstractC2Operator(domain,range)
//    {}

//    TestOperator2(const HilbertSpace& domain, const HilbertSpace& range)
//      : TestOperator2(domain.impl(),range.impl())
//    {}

//    ~TestOperator2(){}

//    std::unique_ptr<AbstractOperator> clone() const
//    {
//      return std::make_unique<TestOperator2>(getDomain(),range());
//    }

//    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement &x) const override
//    {
//      auto result = range().element();
//      result->coefficient(0) = exp(x.coefficient(0))-2*x.coefficient(1);
//      return std::move(result);
//    }

//    std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement &dx) const override
//    {
//      auto result = range().element();
//      result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0) - 2*dx.coefficient(1);
//      return std::move(result);
//    }

//    std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const
//    {
//      auto result = range().element();
//      result->coefficient(0) = exp(x_->coefficient(0))*dx.coefficient(0)*dy.coefficient(0);
//      return std::move(result);
//    }

//  private:
//    mutable const AbstractFunctionSpaceElement* x_;
//  };
}

#endif // ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
