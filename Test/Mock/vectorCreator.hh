// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef MOCK_VECTOR_CREATOR_HH
#define MOCK_VECTOR_CREATOR_HH

namespace Spacy
{
  class Vector;
  class VectorSpace;
}

namespace Mock
{
  class Vector;

  class VectorCreator
  {
  public:
    Vector operator()(const Spacy::VectorSpace* space) const;
  };
}

#endif
