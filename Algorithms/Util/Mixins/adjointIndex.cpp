#include "adjointIndex.hh"

#include <cmath>

Algorithm::Mixin::AdjointIndex::AdjointIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::AdjointIndex::setAdjointIndex(unsigned index) noexcept
{
  index_ = index;
}

double Algorithm::Mixin::AdjointIndex::adjointIndex() const noexcept
{
  return index_;
}
