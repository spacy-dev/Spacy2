// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_HILBERT_SPACE_NORM_HH
#define SPACY_HILBERT_SPACE_NORM_HH

#include "Spacy/scalarProduct.hh"

namespace Spacy
{
  /// \f$ \|\cdot\|=\sqrt{(\cdot,\cdot)} \f$
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
