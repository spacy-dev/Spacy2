#include "stateIndex.hh"

#include <cmath>

VSA::Mixin::StateIndex::StateIndex(unsigned index) noexcept
  : index_(index)
{}

void VSA::Mixin::StateIndex::setStateIndex(unsigned index)
{
  index_ = index;
  notify();
}

double VSA::Mixin::StateIndex::stateIndex() const noexcept
{
  return index_;
}

void VSA::Mixin::StateIndex::update(StateIndex* changedSubject)
{
  setStateIndex( changedSubject->stateIndex() );
}

void VSA::Mixin::StateIndex::attachStateIndex(StateIndex& other)
{
  attach(other);
}

void VSA::Mixin::StateIndex::detachStateIndex(StateIndex& other)
{
  detach(other);
}
