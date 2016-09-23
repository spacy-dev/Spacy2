#pragma once

#include <Spacy/Spaces/RealSpace/real.hh>

namespace Spacy
{
  /// A one-dimensional cubic function \f$q(t) = a + bt + ct^2 + dt^3\f$.
  class Cubic
  {
  public:
    Cubic(Real a, Real b, Real c, Real d) noexcept;

    /// Compute \f$q(t) = a + bt + ct^2 + dt^3 \f$.
    Real operator()(Real t) const noexcept;

  private:
    Real a_, b_, c_, d_;
  };
}
