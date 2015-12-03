// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_C1_FUNCTIONAL_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_C1_FUNCTIONAL_CONCEPT_HH

#include "Spacy/Util/Concepts/functionalConcept.hh"

/**
 * @ingroup ConceptGroup
 * @brief Concept for differentiable functionals in %Spacy.
 *
 * @code
 * // f: X->R.
 * class MyFunctional
 * {
 * public:
 *   // Compute f(x).
 *   Real operator()(const ::Spacy::Vector& x) const;
 *
 *   // Compute f'(x) as element of the dual space X*.
 *   ::Spacy::Vector d1(const ::Spacy::Vector& x) const;
 *
 *   // Access underlying domain space X.
 *   const VectorSpace& domain() const;
 * };
 * @endcode
 *
 * @see FunctionalConcept, ::Spacy::C1Functional
 */
namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  template <class Impl>
  using C1FunctionalConcept =
  std::integral_constant< bool ,
    FunctionalConcept<Impl>::value &&
    HasMemFn_d1_Functional<Impl,Vector>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_C1_FUNCTIONAL_CONCEPT_HH
