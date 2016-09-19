#include "hilbertSpaceNorm.hh"

namespace Spacy
{
  HilbertSpaceNorm::HilbertSpaceNorm(ScalarProduct sp)
    : sp_(std::move(sp))
  {}

  Real HilbertSpaceNorm::operator()(const Vector& x) const
  {
    return sqrt(sp_(x,x));
  }
}
