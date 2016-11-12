#pragma once

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
