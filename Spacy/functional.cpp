#include "functional.hh"

#include "Spacy/operator.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  Vector d1(const C2Functional& f, const Vector& x)
  {
    return f.d1(x);
  }

  LinearOperator d2(const C2Functional& f, const Vector& x)
  {
    return f.hessian(x);
  }
}
