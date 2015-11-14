#include "dampingFactor.hh"

#include <cmath>

namespace Spacy
{
  DampingFactor::DampingFactor(Real nu, double eps)
    : Mixin::Eps(eps)
  {
    set(nu);
  }

  DampingFactor::DampingFactor(double nu, double eps)
    : Mixin::Eps(eps)
  {
    set(nu);
  }

  DampingFactor& DampingFactor::operator=(Real nu)
  {
    set(nu);
    return *this;
  }

  DampingFactor& DampingFactor::operator=(double nu)
  {
    set(nu);
    return *this;
  }

  DampingFactor::operator Real() const
  {
    return nu_;
  }

  Real DampingFactor::operator()() const
  {
    return nu_;
  }

  void DampingFactor::set(Real nu)
  {
    nu_ = ( abs(1-nu) < eps() ) ? 1. : nu;
  }
}
