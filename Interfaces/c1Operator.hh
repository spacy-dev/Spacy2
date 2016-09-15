#pragma once 

#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

namespace Spacy
{
  /// Type-erased differentiable operator \f$A:\ X \to Y \f$.
  class C1Operator
  {
  public:
    /// Apply operator.
    Vector operator()(const Vector& x) const;

    /// Compute directional derivative \f$A'(x)\delta x\f$.
    Vector d1(const Vector &x, const Vector &dx) const;

    /// Get linearization \f$A'(x):\ X\to Y \f$
    LinearOperator linearization(const Vector& x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;
  };



  /**
   * @brief For an operator \f$ A: X\to Y \f$, compute \f$A'\f$ at \f$x\in X\f$ as linear operator \f$ A'(x): X \to Y \f$.
   *
   * Equivalent to calling A.linearization(x).
   *
   * @param A differentiable operator
   * @param x point of linearization
   * @return \f$A'(x)\f$, i.e. A.linearization(x).
   * @see @ref C1OperatorAnchor "C1Operator", @ref LinearOperatorAnchor "LinearOperator"
   */
  LinearOperator d1(const C1Operator& A, const Vector& x)
  {
      return A.linearization(x);
  }
}

