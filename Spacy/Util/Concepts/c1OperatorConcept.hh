// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_C1_OPERATOR_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_C1_OPERATOR_CONCEPT_HH

#include "operatorConcept.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * @ingroup ConceptGroup
   * @brief Concept for differentiable operators in %Spacy.
   *
   * @code
   * // A: X->Y.
   * class MyOperator
   * {
   * public:
   *   // Compute A(x).
   *   ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;
   *
   *   // Compute A'(x)dx.
   *   ::Spacy::Vector d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;
   *
   *   // Get linearization representing A'(x).
   *  LinearOperator linearization(const ::Spacy::Vector& x) const;
   *
   *   // Access domain space X.
   *   const VectorSpace& domain() const;
   *
   *   // Access range space Y.
   *   const VectorSpace& range() const;
   * };
   * @endcode
   *
   * The member function `linearization(x)` is allowed to return any type that satisfies the LinearOperatorConcept.
   *
   * @see OperatorConcept, ::Spacy::C1Operator
   */
  template <class Impl>
  using C1OperatorConcept =
  std::integral_constant< bool ,
    OperatorConcept<Impl>::value &&
    HasMemFn_d1_Operator<Impl,Vector>::value &&
    HasMemFn_linearization<Impl,Vector>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_C1_OPERATOR_CONCEPT_HH
