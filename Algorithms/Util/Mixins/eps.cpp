#include "eps.hh"

#include <cmath>

namespace Algorithm
{
  namespace Mixin
  {

    Eps::Eps(double eps) noexcept
      : eps_(eps)
    {}

    void Eps::setEps(double eps) noexcept
    {
      eps_ = eps;
      notify();
    }

    double Eps::eps() const noexcept
    {
      return eps_;
    }

    double Eps::sqrtEps() const noexcept
    {
      return sqrt(eps_);
    }

    double Eps::cbrtEps() const noexcept
    {
      return cbrt(eps_);
    }

    void Eps::update(DesignPattern::Observer::Subject *changedSubject)
    {
      if( changedSubject == this ) return;
      setEps( dynamic_cast<Eps*>(changedSubject)->eps() );
    }

    void Eps::attachEps(Eps* other)
    {
      attach(other);
    }

    void Eps::detachEps(Eps* other)
    {
      detach(other);
    }
  }
}
