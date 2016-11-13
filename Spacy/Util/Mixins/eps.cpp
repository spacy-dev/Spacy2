#include "eps.hh"

#include <cmath>

namespace Spacy
{
  namespace Mixin
  {

    Eps::Eps(Real eps) noexcept
      : eps_(eps)
    {}

    void Eps::setEps(Real eps)
    {
      eps_ = eps;
      notify();
    }

    Real Eps::eps() const noexcept
    {
      return eps_;
    }

    Real Eps::sqrtEps() const noexcept
    {
      return sqrt(eps_);
    }

    Real Eps::cbrtEps() const noexcept
    {
      return cbrt(eps_);
    }

    void Eps::update(Eps* changedSubject)
    {
      setEps( changedSubject->eps() );
    }
  }
}
