#pragma once

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/linearSolver.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>
#include <functional>

namespace Spacy
{
  /// Type-erased linear operator \f$A:\ X \to Y \f$.
  class LinearOperator
  {
  public:
    /// Apply operator.
    Vector operator()(const Vector& x) const;

    Real operator()(const LinearOperator& x) const;

    LinearOperator& operator+=(const LinearOperator& y);

    LinearOperator& operator-=(const LinearOperator& y);

    LinearOperator& operator*=(double a);

    LinearOperator operator-() const;

    bool operator==(const LinearOperator& y) const;

    std::function<Vector(const Vector&)> solver() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Access underlying space of linear operators.
    const VectorSpace& space() const;
  };

  /// Access solver via A^-1. Throws for k!=-1.
  inline LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw Exception::InvalidArgument("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  /// Access solver via A^-1. Throws for k!=-1.
  inline LinearSolver operator^(LinearOperator&& A, int k)
  {
    if( k == -1 ) return std::move(A.solver());
    throw Exception::InvalidArgument("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  inline LinearOperator& axpy(LinearOperator& A, double a, LinearOperator B)
  {
    return A += ( B *= a );
  }
}
