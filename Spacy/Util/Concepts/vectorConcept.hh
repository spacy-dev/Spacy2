// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  /**
   * @ingroup ConceptGroup
   * @brief Concept for vectors in %Spacy.
   *
   * @code
   * class MyVector
   * {
   * public:
   *   // Copy constructor.
   *   MyVector(const MyVector&);
   *
   *   // Copy assignable x=y.
   *   MyVector& operator=(const MyVector& y);
   *
   *   // In-place summation x+=y.
   *   MyVector& operator+=(const MyVector& y);
   *
   *   // In-place subtraction x-=y.
   *   MyVector& operator-=(const MyVector& y);
   *
   *   // In-place multiplication x*=a.
   *   MyVector& operator*=(double a);
   *
   *   // Negation -y.
   *   MyVector operator-(const MyVector& y);
   *
   *   // Equality comparison x==y.
   *   bool operator==(const MyVector& y);
   *
   *   // Apply as dual element x(y).
   *   Real operator()(const MyVector& y);
   *
   *   // Access pointer to underlying function space.
   *   const VectorSpace& space() const;
   * };
   * @endcode
   *
   * @see ::Spacy::Vector
   */
  template <class Impl>
  using VectorConcept =
  std::integral_constant< bool ,
// The following line does not compile with gcc-5.2.0
//    HasMemOp_callable<Impl,Impl,Real>::value &&
    std::is_copy_constructible<Impl>::value &&
    std::is_copy_assignable<Impl>::value &&
    HasMemOp_add<Impl>::value &&
    HasMemOp_subtract<Impl>::value &&
    HasMemOp_multiply<Impl>::value &&
    HasMemOp_negate<Impl>::value &&
    HasMemFn_space<Impl>::value &&
    IsVoid< Impl >::template apply<TryMemFn_toFile>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH
