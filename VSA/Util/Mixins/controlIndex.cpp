#include "controlIndex.hh"

#include <cmath>

VSA::Mixin::ControlIndex::ControlIndex(unsigned index) noexcept
  : index_(index)
{}

void VSA::Mixin::ControlIndex::setControlIndex(unsigned index)
{
  index_ = index;
  notify();
}

double VSA::Mixin::ControlIndex::controlIndex() const noexcept
{
  return index_;
}

void VSA::Mixin::ControlIndex::update(ControlIndex* changedSubject)
{
  setControlIndex( changedSubject->controlIndex() );
}

void VSA::Mixin::ControlIndex::attachControlIndex(ControlIndex& other)
{
  attach(other);
}

void VSA::Mixin::ControlIndex::detachControlIndex(ControlIndex& other)
{
  detach(other);
}
