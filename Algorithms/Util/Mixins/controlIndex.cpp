#include "controlIndex.hh"

#include <cmath>

Algorithm::Mixin::ControlIndex::ControlIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::ControlIndex::setControlIndex(unsigned index) noexcept
{
  index_ = index;
}

double Algorithm::Mixin::ControlIndex::controlIndex() const noexcept
{
  return index_;
}
