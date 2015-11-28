#include "functional.hh"

#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

#include <cassert>

namespace Spacy
{
  Real Functional::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
//    assert(apply_);
//    return apply_(x);
  }

  const VectorSpace& Functional::domain() const
  {
    assert(base_);
    return base_->domain();
//    assert(domain_);
//    return domain_();
  }

  Functional::operator bool() const
  {
    return base_;
//    return !is_empty(*this);
  }


  Real C1Functional::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
//    assert(apply_);
//    return apply_(x);
  }

  Vector C1Functional::d1(const Vector& x) const
  {
    assert(base_);
    return base_->d1(x);
//    assert(d1_);
//    return d1_(x);
  }

  const VectorSpace& C1Functional::domain() const
  {
    assert(base_);
    return base_->domain();
//    assert(domain_);
//    return domain_();
  }

  C1Functional::operator bool() const
  {
    return base_;
//    return !is_empty(*this);
  }


  Real C2Functional::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
//    assert(apply_);
//    return apply_(x);
  }

  Vector C2Functional::d1(const Vector& x) const
  {
    assert(base_);
    return base_->d1(x);
//    assert(d1_);
//    return d1_(x);
  }

  Vector C2Functional::d2(const Vector& x, const Vector& dx) const
  {
    assert(base_);
    return base_->d2(x,dx);
//    assert(d2_);
//    return d2_(x,dx);
  }

  LinearOperator C2Functional::hessian(const Vector& x) const
  {
    assert(base_);
    return base_->hessian(x);
//    assert(hessian_);
//    return hessian_(x);
  }

  const VectorSpace& C2Functional::domain() const
  {
    assert(base_);
    return base_->domain();
//    assert(domain_);
//    return domain_();
  }

  C2Functional::operator bool() const
  {
    return base_;
//    return !is_empty(*this);
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
