#pragma once 

#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/RealSpace/real.hh>

namespace Spacy
{
  /// Type-erased twice differentiable functional \f$f:\ X \to \mathbb{R} \f$.
  class C2Functional
  {
  public:
    /// Apply functional.
    Real operator()(const Vector& x) const;

    /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
    Vector d1(const Vector &x) const;

    /// Compute second derivative as function space element in \f$X^*\f$, where \f$x,dx\in X\f$.
    Vector d2(const Vector &x, const Vector& dx) const;

    /// Access hessian as linear operator \f$ X \rightarrow X^* \f$.
    LinearOperator hessian(const Vector &x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;
  };


  /**
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f'\f$ at \f$x\in X\f$ as dual element \f$ f'(x) \in X^* \f$.
   *
   * Equivalent to calling f.d1(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f'(x)\f$, i.e. f.d1(x).
   * @see C2Functional
   */
  Vector d1(const C2Functional& f, const Vector& x)
  {
    return f.d1(x);
  }

  /**
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f''\f$ at \f$x\in X\f$ as linear operator \f$ f''(x): X \to X^* \f$.
   *
   * Equivalent to calling f.hessian(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f''(x)\f$, i.e. f.hessian(x).
   * @see C2Functional
   */
  LinearOperator d2(const C2Functional& f, const Vector& x)
  {
    return f.hessian(x);
  }
}

#endif // SPACY_C2_FUNCTIONAL_HH
