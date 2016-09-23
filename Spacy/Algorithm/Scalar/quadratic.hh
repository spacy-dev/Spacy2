#pragma once

#include <Spacy/Spaces/RealSpace/real.hh>

namespace Spacy
{
  ///A one-dimensional quadratic function \f$q(t) = a + bt + ct^2\f$.
  class Quadratic
  {
  public:
    Quadratic(Real a, Real b, Real c) noexcept;

    /// Compute \f$q(t) = a + bt + ct^2 \f$.
    Real operator()(Real t) const noexcept;

  private:
    Real a_, b_, c_;
  };
}
