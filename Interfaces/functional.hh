#pragma once

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/RealSpace/real.hh>

namespace Spacy
{
  /// Type-erased functional \f$f:\ X \to \mathbb{R} \f$.
  class Functional
  {
  public:
    /// Apply functional.
    Real operator()(const Vector& x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;
  };
}

