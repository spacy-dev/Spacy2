#include "addEps.hh"

void Algorithm::AddEps::setEps(double eps) noexcept
{
  eps_ = eps;
}

double Algorithm::AddEps::eps() const noexcept
{
  return eps_;
}
