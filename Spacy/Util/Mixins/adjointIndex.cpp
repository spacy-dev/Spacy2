#include "adjointIndex.hh"

#include <cmath>

Spacy::Mixin::AdjointIndex::AdjointIndex(unsigned index) noexcept
  : index_(index)
{}

void Spacy::Mixin::AdjointIndex::setAdjointIndex(unsigned index)
{
  index_ = index;
  notify();
}

double Spacy::Mixin::AdjointIndex::adjointIndex() const noexcept
{
  return index_;
}

void Spacy::Mixin::AdjointIndex::update(AdjointIndex* changedSubject)
{
  setAdjointIndex( changedSubject->adjointIndex() );
}

void Spacy::Mixin::AdjointIndex::attachAdjointIndex(AdjointIndex& other)
{
  attach(other);
}

void Spacy::Mixin::AdjointIndex::detachAdjointIndex(AdjointIndex& other)
{
  detach(other);
}
