#include "adjointIndex.hh"

#include <cmath>

VSA::Mixin::AdjointIndex::AdjointIndex(unsigned index) noexcept
  : index_(index)
{}

void VSA::Mixin::AdjointIndex::setAdjointIndex(unsigned index)
{
  index_ = index;
  notify();
}

double VSA::Mixin::AdjointIndex::adjointIndex() const noexcept
{
  return index_;
}

void VSA::Mixin::AdjointIndex::update(AdjointIndex* changedSubject)
{
  setAdjointIndex( changedSubject->adjointIndex() );
}

void VSA::Mixin::AdjointIndex::attachAdjointIndex(AdjointIndex& other)
{
  attach(other);
}

void VSA::Mixin::AdjointIndex::detachAdjointIndex(AdjointIndex& other)
{
  detach(other);
}
