#pragma once

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

namespace Spacy
{
  /// Type-erased differentiable functional \f$f:\ X \to \mathbb{R} \f$.
  class C1Functional
  {
  public:
    /// Apply functional.
    Real operator()(const Vector& x) const;

    /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
    Vector d1(const Vector &x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;
  };
}

