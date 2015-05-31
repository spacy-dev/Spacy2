#include "jacobipreconditioner.hh"

namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const C0Operator& A)
    : AbstractC0Operator(A.impl().getDomain(),A.impl().getRange()),
      diag_(A.impl().getDomain().element()->size(),0.)

  {
    auto x = A.impl().getDomain().element();
    for(unsigned int i=0; i<x->size(); ++i)
    {
      x->coefficient(i) = 1;
      diag_[i] = 1/(A.impl()(*x))->coefficient(i);
      x->coefficient(i) = 0;
    }
  }

//  void JacobiPreconditioner::setArgument(const FunctionSpaceElement& x)
//  {
//    result_ = x;
//    for(unsigned i = 0; i<x.size(); ++i)
//      result_.coefficient(i) *= diag_[i];
//  }


  std::unique_ptr<AbstractC0Operator> JacobiPreconditioner::clone() const
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
}
