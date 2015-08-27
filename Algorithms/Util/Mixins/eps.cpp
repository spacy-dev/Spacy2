#include "eps.hh"

#include <cmath>

namespace Algorithm
{
  namespace Mixin
  {

    Eps::Eps(double eps) noexcept
      : eps_(eps)
    {}

    void Eps::setEps(double eps)
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

    void Eps::update(Eps* changedSubject)
    {
      setEps( changedSubject->eps() );
    }

    void Eps::attachEps(Eps& other)
    {
      attach(other);
    }

    void Eps::detachEps(Eps& other)
    {
      detach(other);
    }
  }
}
