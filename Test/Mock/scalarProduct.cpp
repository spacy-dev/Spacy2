#include "scalarProduct.hh"

Spacy::Real Mock::ScalarProduct::operator()(const ::Spacy::Vector&, const ::Spacy::Vector&) const
{
  return Spacy::Real(testValue);
}

