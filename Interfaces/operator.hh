#pragma once 

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <functional>

namespace Spacy
{
  /**
   * An operator that does not know about domain and range spaces. 
   * Good enough for some algorithms (i.e. for Krylov-methods).
   */
  using CallableOperator = std::function< Vector (const Vector&) >;
  
  /// Type-erased operator \f$A:\ X \to Y \f$.
  class Operator
  {
  public:
    /// Apply operator.
    Vector operator()(const Vector& x) const;
    
    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;
    
    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;
  };
}

