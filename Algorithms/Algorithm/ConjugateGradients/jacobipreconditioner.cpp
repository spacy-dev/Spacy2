//#include "jacobipreconditioner.hh"


//namespace Algorithm
//{
//  using Interface::AbstractOperator;

//  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
//    : AbstractOperator(A.impl().domain_ptr(),A.impl().range_ptr()),
//      diag_(A.impl().domain().element().size(),1.)

//  {
//    auto x = domain().element();
//    auto B = A;
//    for(unsigned int i=0; i<x.size(); ++i)
//    {
//      x.coefficient(i) = 1;
//      diag_[i] = 1/(B.impl()(x)).coefficient(i);
//      x.coefficient(i) = 0;
//    }
//  }

//  JacobiPreconditioner::JacobiPreconditioner(const LinearOperator& A)
//    : AbstractOperator(A.impl().domain_ptr(),A.impl().range_ptr()),
//      diag_(A.impl().domain().element().size(),1.)

//  {
//    auto x = domain().element();
//    auto B = A;
//    for(unsigned int i=0; i<x.size(); ++i)
//    {
//      x.coefficient(i) = 1;
//      diag_[i] = 1/(B.impl()(x)).coefficient(i);
//      x.coefficient(i) = 0;
//    }
//  }


//  Vector JacobiPreconditioner::operator()(const Vector& x) const
//  {
//    auto y = range().element();
//    for(unsigned i = 0; i < x.size(); ++i)
//      y->coefficient(i) *= diag_[i];
//    return y;
//  }

//  JacobiPreconditioner* JacobiPreconditioner::cloneImpl() const
//  {
//    return new JacobiPreconditioner(*this);
//  }


//  Operator jacobiPreconditioner(const Operator& A)
//  {
//    return Operator(std::make_unique<JacobiPreconditioner>(A));
//  }

//  Operator jacobiPreconditioner(const LinearOperator& A)
//  {
//    return Operator(std::make_unique<JacobiPreconditioner>(A));
//  }
//}
