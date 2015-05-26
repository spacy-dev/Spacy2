#ifndef ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
#define ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH

#include <cmath>

#include "../operator.hh"
#include "../functionSpace.hh"
#include "../functionSpaceElement.hh"
#include "../FunctionSpaces/RealNumbers/real.hh"

namespace Algorithm
{
  template <class Function>
  class ScalarRFFGenOperator : public Operator
  {
  public:
    ScalarRFFGenOperator(const Function& f, const FunctionSpace& space)
      : Operator(space,space), f_(f)
    {}

    ~ScalarRFFGenOperator(){}

    void update(const FunctionSpaceElement &x) override
    {
      f_.update(x.coefficient(0));
    }

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const override
    {
      return makeElement<Real>( f_(x.coefficient(0)) , this->getRange().impl() );
    }

    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
    {
      return makeElement<Real>( f_.template d1<0>(dx.coefficient(0.)) , this->getRange().impl() );
    }

  private:
    mutable Function f_;
  };

  template <class Function>
  auto makeScalarRFFGenOperator(const Function& f, const FunctionSpace& space)
  {
    return ScalarRFFGenOperator<Function>(f,space);
  }

  class TestOperator : public Operator
  {
  public:
    TestOperator(const FunctionSpace& space)
      : Operator(space,space)
    {}

    ~TestOperator(){}

    void update(const FunctionSpaceElement &x) override
    {
      x_ = x;
    }

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const override
    {
      x_ = x;
      return makeElement<Real>( exp(x_.coefficient(0))-2 , this->getRange().impl() );
    }

    FunctionSpaceElement d1(const FunctionSpaceElement &dx) const override
    {
      return makeElement<Real>( exp(x_.coefficient(0))*dx.coefficient(0) , this->getRange().impl() );
    }

  private:
    mutable FunctionSpaceElement x_;
  };
}

#endif // ALGORITHM_TEST_SCALAR_RFFGEN_OPERATOR_HH
