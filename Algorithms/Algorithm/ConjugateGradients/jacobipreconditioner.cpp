#include "jacobipreconditioner.hh"

namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
    : diag_(A.getDomain().element().size(),0.),
      domain_(A.getDomain()),
      range_(A.getRange())

  {
    auto x = A.getDomain().element();
    for(unsigned int i=0; i<x.size(); ++i)
    {
      x.coefficient(i) = 1;
      diag_[i] = 1/A(x).coefficient(i);
      x.coefficient(i) = 0;
    }
  }

//  void JacobiPreconditioner::setArgument(const FunctionSpaceElement& x)
//  {
//    result_ = x;
//    for(unsigned i = 0; i<x.size(); ++i)
//      result_.coefficient(i) *= diag_[i];
//  }


  std::unique_ptr<AbstractOperator> JacobiPreconditioner::clone() const
  {
    return std::make_unique<JacobiPreconditioner>(*this);
  }


  std::unique_ptr<AbstractFunctionSpaceElement> JacobiPreconditioner::operator()(const AbstractFunctionSpaceElement& x) const
  {
    auto y = x.clone();
    for(unsigned i = 0; i < x.size(); ++i)
      y->coefficient(i) *= diag_[i];
    return y;
  }

  const FunctionSpace& JacobiPreconditioner::getDomain() const
  {
    return domain_;
  }

  const FunctionSpace& JacobiPreconditioner::getRange() const
  {
    return range_;
  }
}
