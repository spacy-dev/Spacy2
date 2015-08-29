#include "dampingFactor.hh"

#include <cmath>

namespace Algorithm
{
  DampingFactor::DampingFactor(double nu, double eps)
    : Mixin::Eps(eps)
  {
    set(nu);
  }

  DampingFactor& DampingFactor::operator=(double nu)
  {
    set(nu);
    return *this;
  }

  DampingFactor::operator double() const
  {
    return nu_;
  }

  void DampingFactor::set(double nu)
  {
    nu_ = ( std::abs(1-nu) < eps() ) ? 1. : nu;
  }
}
