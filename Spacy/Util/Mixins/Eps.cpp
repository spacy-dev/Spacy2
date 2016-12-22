#include "Eps.hh"

namespace Spacy
{
  namespace Mixin
  {

    Eps::Eps(Real eps) noexcept
      : eps_(eps)
    {}

    void Eps::set_eps(Real eps)
    {
      eps_ = eps;
      notify();
    }

    Real Eps::eps() const noexcept
    {
      return eps_;
    }

    Real Eps::sqrt_eps() const noexcept
    {
      return sqrt(eps_);
    }

    Real Eps::cbrt_eps() const noexcept
    {
      return cbrt(eps_);
    }

    void Eps::update(Eps* changedSubject)
    {
      set_eps( changedSubject->eps() );
    }
  }
}
