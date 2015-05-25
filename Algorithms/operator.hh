#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include "functionSpace.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class Operator
  {
  public:
    Operator(const FunctionSpace& domain, const FunctionSpace& range);

    virtual ~Operator(){}

    virtual void update(const FunctionSpaceElement& x);

    virtual FunctionSpaceElement operator()(const FunctionSpaceElement& x) const = 0;

    const FunctionSpace& getRange() const;

    const FunctionSpace& getDomain() const;

    virtual FunctionSpaceElement d1(const FunctionSpaceElement&) const;

  protected:
    const FunctionSpace& domain_;
    const FunctionSpace& range_;
  };


  class Functional
  {
  public:
    Functional(const FunctionSpace& domain, const FunctionSpace& range);

    virtual ~Functional(){}

    virtual void setArgument(const FunctionSpaceElement& x) = 0;

    virtual FunctionSpaceElement operator()(const FunctionSpaceElement& y) const = 0;

    virtual FunctionSpaceElement d1(const FunctionSpaceElement& y, const FunctionSpaceElement& dy) const = 0;

    const FunctionSpace& getRange() const;

    const FunctionSpace& getDomain() const;

  private:
    const FunctionSpace& domain_;
    const FunctionSpace& range_;
  };
}
#endif // ALGORITHM_OPERATOR_HH
