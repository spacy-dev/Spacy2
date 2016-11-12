#include "vectorCreator.hh"

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

#include "vector.hh"

Mock::Vector Mock::VectorCreator::operator()(const Spacy::VectorSpace* space) const
{
  return Mock::Vector{*space};
}
