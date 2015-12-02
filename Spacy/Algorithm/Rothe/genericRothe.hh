// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITHM_GENERIC_ROTHE_HH
#define SPACY_ALGORITHM_GENERIC_ROTHE_HH

namespace Spacy
{
  /// @cond
  class DynamicC1Operator;
  class Vector;
  /// @endcond

  namespace Rothe
  {
    Vector genericMethod(const DynamicC1Operator& A, double t0=0, double t1=1);
  }
}

#endif // SPACY_ALGORITHM_GENERIC_ROTHE_HH
