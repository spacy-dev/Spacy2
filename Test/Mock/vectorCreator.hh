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
