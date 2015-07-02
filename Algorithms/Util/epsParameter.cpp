#include "epsParameter.hh"

void Algorithm::EpsParameter::setEps(double eps) noexcept
{
  eps_ = eps;
}

double Algorithm::EpsParameter::eps() const noexcept
{
  return eps_;
}
