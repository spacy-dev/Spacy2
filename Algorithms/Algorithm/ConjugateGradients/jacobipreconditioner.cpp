#include "jacobipreconditioner.hh"

namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
    : diag_(A.getDomain().element().size(),0.),
      domain_(A.getDomain()),
      range_(A.getRange()),
      result_(range_.element())

  {
    auto x = A.getDomain().element();
    for(unsigned int i=0; i<x.size(); ++i)
    {
      x.coefficient(i) = 1;
      diag_[i] = 1/A(x).coefficient(i);
      x.coefficient(i) = 0;
    }
  }

  void JacobiPreconditioner::setArgument(const FunctionSpaceElement& x)
  {
    result_ = x;
    for(unsigned i = 0; i<x.size(); ++i)
      result_.coefficient(i) *= diag_[i];
  }

  FunctionSpaceElement JacobiPreconditioner::operator()() const
  {
    return result_;
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
