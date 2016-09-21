#pragma once 

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

namespace Spacy
{  
  /// Each VectorSpace needs a zero-vector creator to support generation of vector space elements.
  class ZeroVectorCreator
  {
  public:
    /// Creates \f$ 0 \in V \f$
    Vector operator()(const VectorSpace* V) const;
  };
    

  template <class T>
  T& creator(VectorSpace& space)
  {
    return *space.creator().template target<T>();
  }

  template <class T>
  const T& creator(const VectorSpace& space)
  {
    return *space.creator().template target<T>();
  }

  /// Create new vector \f$v=0\f$.
  Vector zero(const VectorSpace& space)
  {
      return space.creator()(&space);
  }
}

