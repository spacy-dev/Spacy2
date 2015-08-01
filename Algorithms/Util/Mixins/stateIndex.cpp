#include "stateIndex.hh"

#include <cmath>

Algorithm::Mixin::StateIndex::StateIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::StateIndex::setStateIndex(unsigned index) noexcept
{
  index_ = index;
  connection_.forward(index_);
}

double Algorithm::Mixin::StateIndex::stateIndex() const noexcept
{
  return index_;
}
