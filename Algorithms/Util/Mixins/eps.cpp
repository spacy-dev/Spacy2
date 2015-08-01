#include "eps.hh"

#include <cmath>

Algorithm::Mixin::Eps::Eps(double eps) noexcept
  : eps_(eps)
{}

void Algorithm::Mixin::Eps::setEps(double eps) noexcept
{
  eps_ = eps;
  connection_.forward(eps_);
}

double Algorithm::Mixin::Eps::eps() const noexcept
{
  return eps_;
}

double Algorithm::Mixin::Eps::sqrtEps() const noexcept
{
  return sqrt(eps_);
}

double Algorithm::Mixin::Eps::cbrtEps() const noexcept
{
  return cbrt(eps_);
}
