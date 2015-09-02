#include "controlIndex.hh"

#include <cmath>

Spacy::Mixin::ControlIndex::ControlIndex(unsigned index) noexcept
  : index_(index)
{}

void Spacy::Mixin::ControlIndex::setControlIndex(unsigned index)
{
  index_ = index;
  notify();
}

double Spacy::Mixin::ControlIndex::controlIndex() const noexcept
{
  return index_;
}

void Spacy::Mixin::ControlIndex::update(ControlIndex* changedSubject)
{
  setControlIndex( changedSubject->controlIndex() );
}

void Spacy::Mixin::ControlIndex::attachControlIndex(ControlIndex& other)
{
  attach(other);
}

void Spacy::Mixin::ControlIndex::detachControlIndex(ControlIndex& other)
{
  detach(other);
}
