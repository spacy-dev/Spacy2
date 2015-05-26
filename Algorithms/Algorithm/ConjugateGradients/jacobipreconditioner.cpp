#include "jacobipreconditioner.hh"

#include "../Util/callofundefinedfunctionexception.hh"

namespace Algorithm
{
  JacobiPreconditioner::JacobiPreconditioner(const Operator& A)
    : Operator( A.getRange() , A.getDomain() ),
      diag(A.getDomain().element().size(),0.)
  {
    auto x = A.getDomain().element();
    for(unsigned int i=0; i<x.size(); ++i)
    {
      x.coefficient(i) = 1;
      diag[i] = 1/A(x).coefficient(i);
      x.coefficient(i) = 0;
    }
  }

  void JacobiPreconditioner::update(const FunctionSpaceElement& x)
  {
    throw CallOfUndefinedFunctionException("JacobiPreconditioner::update");
  }

  FunctionSpaceElement JacobiPreconditioner::operator()(const FunctionSpaceElement& x) const
  {
    auto y = x;

    for(unsigned i = 0; i<x.size(); ++i)
      y.coefficient(i) *= diag[i];

    return y;
  }

  FunctionSpaceElement JacobiPreconditioner::d1(const FunctionSpaceElement& dx) const
  {
    throw CallOfUndefinedFunctionException("JacobiPreconditioner::d1");
  }
}
