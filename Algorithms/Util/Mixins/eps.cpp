#include "eps.hh"

Algorithm::Mixin::Eps::Eps(double eps)
  : eps_(eps)
{}

void Algorithm::Mixin::Eps::setEps(double eps) noexcept
{
  eps_ = eps;
}

double Algorithm::Mixin::Eps::eps() const noexcept
{
  return eps_;
}
