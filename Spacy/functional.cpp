#include "functional.hh"

#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  Real Functional::operator()(const Vector& x) const
  {
    return base_.impl()(x);
  }

  const VectorSpace& Functional::domain() const
  {
    return base_.impl().domain();
  }

  Functional::operator bool() const
  {
    return !is_empty(base_);
  }


  Real C1Functional::operator()(const Vector& x) const
  {
    return base_.impl()(x);
  }

  Vector C1Functional::d1(const Vector& x) const
  {
    return base_.impl().d1(x);
  }

  const VectorSpace& C1Functional::domain() const
  {
    return base_.impl().domain();
  }

  C1Functional::operator bool() const
  {
    return !is_empty(base_);
  }


  Vector d1(const C2Functional& f, const Vector& x)
  {
    return f.d1(x);
  }

  LinearOperator d2(const C2Functional& f, const Vector& x)
  {
    return f.hessian(x);
  }
}
