#include "jacobipreconditioner.hh"


namespace Algorithm
{
  using Interface::AbstractVector;
  using Interface::AbstractOperator;

  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
    : AbstractOperator(A.impl().domain_ptr(),A.impl().range_ptr()),
      diag_(A.impl().domain().element().size(),1.)

  {
    auto x = A.impl().domain().element();
    auto B = A;
    for(unsigned int i=0; i<x.impl().size(); ++i)
    {
      x.impl().coefficient(i) = 1;
      diag_[i] = 1/(B.impl()(x.impl()))->coefficient(i);
      x.impl().coefficient(i) = 0;
    }
  }

  JacobiPreconditioner::JacobiPreconditioner(const LinearOperator& A)
    : AbstractOperator(A.impl().domain_ptr(),A.impl().range_ptr()),
      diag_(A.impl().domain().element().size(),1.)

  {
    auto x = A.impl().domain().element();
    auto B = A;
    for(unsigned int i=0; i<x.impl().size(); ++i)
    {
      x.impl().coefficient(i) = 1;
      diag_[i] = 1/(B.impl()(x.impl()))->coefficient(i);
      x.impl().coefficient(i) = 0;
    }
  }


  JacobiPreconditioner::JacobiPreconditioner(const JacobiPreconditioner& other)
    : AbstractOperator(other.domain_ptr(), other.range_ptr()),
      diag_(other.diag_), x_( clone(*other.x_))
  {}


  std::unique_ptr<AbstractVector> JacobiPreconditioner::operator()(const AbstractVector& x) const
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


  Operator jacobiPreconditioner(const Operator& A)
  {
    return Operator(std::make_unique<JacobiPreconditioner>(A));
  }

  Operator jacobiPreconditioner(const LinearOperator& A)
  {
    return Operator(std::make_unique<JacobiPreconditioner>(A));
  }
}
