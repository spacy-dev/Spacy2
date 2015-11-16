#ifndef SPACY_HILBERT_SPACE_NORM_HH
#define SPACY_HILBERT_SPACE_NORM_HH

#include "Spacy/scalarProduct.hh"

namespace Spacy
{
  class HilbertSpaceNorm
  {
  public:
    explicit HilbertSpaceNorm(ScalarProduct sp);

    Real operator()(const Vector& x) const;

  private:
    ScalarProduct sp_;
  };
}

#endif // SPACY_HILBERT_SPACE_NORM_HH
