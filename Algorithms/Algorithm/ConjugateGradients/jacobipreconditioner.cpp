#include "jacobipreconditioner.hh"


namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
    : AbstractOperator(A.impl().getSharedDomain(),A.impl().getSharedRange()),
      diag_(A.impl().getDomain().element()->size(),1.)

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

  JacobiPreconditioner::JacobiPreconditioner(const LinearOperator& A)
    : AbstractOperator(A.impl().getSharedDomain(),A.impl().getSharedRange()),
      diag_(A.impl().getDomain().element()->size(),1.)

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
    : AbstractOperator(other.getSharedDomain(), other.getSharedRange()),
      diag_(other.diag_), x_( clone(other.x_))
  {}


  void JacobiPreconditioner::setArgument(const AbstractFunctionSpaceElement& x)
  {
    x_ = clone(x);
  }

//  std::unique_ptr<AbstractFunctionSpaceElement> JacobiPreconditioner::d0() const
//  {
//    auto y = x_->clone();
//    for(unsigned i = 0; i < y->size(); ++i)
//      y->coefficient(i) *= diag_[i];
//    return y;
//  }

  Operator jacobiPreconditioner(const Operator& A)
  {
    return Operator(std::make_unique<JacobiPreconditioner>(A));
  }

  Operator jacobiPreconditioner(const LinearOperator& A)
  {
    return Operator(std::make_unique<JacobiPreconditioner>(A));
  }

  std::unique_ptr<AbstractFunctionSpaceElement> JacobiPreconditioner::operator()(const AbstractFunctionSpaceElement& x) const
  {
    auto y = clone(x);
    for(unsigned i = 0; i < x.size(); ++i)
      y->coefficient(i) *= diag_[i];
    return y;
  }

  JacobiPreconditioner* JacobiPreconditioner::cloneImpl() const
  {
    return new JacobiPreconditioner(*this);
  }
}
