#pragma once

#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <functional>

namespace Spacy
{
  /**
   * A time-dependent operator that does not know about domain and range spaces.
   */
  using DynamicCallableOperator = std::function<Vector(double,const Vector&)>;


  /// Type-erased time-dependent operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicOperator
  {
  public:
    /// Apply operator.
    Vector operator()(const Vector& x) const;

    LinearOperator M() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;
  };


  /// Type-erased time-dependent linear operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicLinearOperator
  {
  public:
    /// Apply operator.
    Vector operator()(double t, const Vector& x) const;

//    Vector operator()(const DynamicLinearOperator& x) const;

    DynamicLinearOperator& operator+=(const DynamicLinearOperator& y);

    DynamicLinearOperator& operator-=(const DynamicLinearOperator& y);

    DynamicLinearOperator& operator*=(double a);

    DynamicLinearOperator operator-() const;

    bool operator==(const DynamicLinearOperator& y) const;

    std::function<Vector(const Vector&)> solver() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Access underlying space of linear operators.
    const VectorSpace& space() const;
  };


  /// Type-erased time-dependent differentiable operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicC1Operator
  {
  public:
    /// Apply operator.
    Vector operator()(double t, const Vector& x) const;

    /// Compute directional derivative \f$A'(x)\delta x\f$.
    Vector d1(double t, const Vector &x, const Vector &dx) const;

    /// Get linearization \f$A'(x):\ X\to Y \f$
    LinearOperator linearization(double t, const Vector& x) const;

    LinearOperator M() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;
  };
}


