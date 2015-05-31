#include "jacobipreconditioner.hh"

namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const C0Operator& A)
    : AbstractC0Operator(A.impl().getDomain(),A.impl().getRange()),
      diag_(A.impl().getDomain().element()->size(),0.)

  {
    auto x = A.impl().getDomain().element();
    auto B = A;
    for(unsigned int i=0; i<x->size(); ++i)
    {
      x->coefficient(i) = 1;
      diag_[i] = 1/(B.impl()(*x))->coefficient(i);
      x->coefficient(i) = 0;
    }
  }

  JacobiPreconditioner::JacobiPreconditioner(const JacobiPreconditioner& other)
    : AbstractC0Operator(other.getDomain(), other.getRange()),
      diag_(other.diag_), x_(other.x_->clone())
  {}


  void JacobiPreconditioner::setArgument(const AbstractFunctionSpaceElement& x)
  {
    x_ = x.clone();
  }


  std::unique_ptr<AbstractC0Operator> JacobiPreconditioner::clone() const
  {
    return std::make_unique<JacobiPreconditioner>(*this);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> JacobiPreconditioner::d0() const
  {
    auto y = x_->clone();
    for(unsigned i = 0; i < y->size(); ++i)
      y->coefficient(i) *= diag_[i];
    return y;
  }

//  std::unique_ptr<AbstractFunctionSpaceElement> JacobiPreconditioner::operator()(const AbstractFunctionSpaceElement& x) const
//  {
//    auto y = x.clone();
//    for(unsigned i = 0; i < x.size(); ++i)
//      y->coefficient(i) *= diag_[i];
//    return y;
//  }
}
