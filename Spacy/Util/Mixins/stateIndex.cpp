#include "stateIndex.hh"

#include <cmath>

Spacy::Mixin::StateIndex::StateIndex(unsigned index) noexcept
  : index_(index)
{}

void Spacy::Mixin::StateIndex::setStateIndex(unsigned index)
{
  index_ = index;
  notify();
}

double Spacy::Mixin::StateIndex::stateIndex() const noexcept
{
  return index_;
}

void Spacy::Mixin::StateIndex::update(StateIndex* changedSubject)
{
  setStateIndex( changedSubject->stateIndex() );
}

void Spacy::Mixin::StateIndex::attachStateIndex(StateIndex& other)
{
  attach(other);
}

void Spacy::Mixin::StateIndex::detachStateIndex(StateIndex& other)
{
  detach(other);
}
