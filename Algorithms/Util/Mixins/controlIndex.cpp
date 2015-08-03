#include "controlIndex.hh"

#include <cmath>

Algorithm::Mixin::ControlIndex::ControlIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::ControlIndex::setControlIndex(unsigned index) noexcept
{
  index_ = index;
  notify();
}

double Algorithm::Mixin::ControlIndex::controlIndex() const noexcept
{
  return index_;
}

void Algorithm::Mixin::ControlIndex::update(DesignPattern::Observer::Subject *changedSubject)
{
  if( changedSubject == this ) return;
  setControlIndex( dynamic_cast<ControlIndex*>(changedSubject)->controlIndex() );
}

void Algorithm::Mixin::ControlIndex::attachControlIndex(ControlIndex* other)
{
  attach(other);
}

void Algorithm::Mixin::ControlIndex::detachControlIndex(ControlIndex* other)
{
  detach(other);
}
