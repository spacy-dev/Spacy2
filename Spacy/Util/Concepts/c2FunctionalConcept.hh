// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_C2_FUNCTIONAL_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_C2_FUNCTIONAL_CONCEPT_HH

#include "Spacy/Util/Concepts/c1FunctionalConcept.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * @ingroup ConceptGroup
   * @anchor C2FunctionalConceptAnchor
   *
   * @code
   * // f: X->R.
   * class MyFunctional
   * {
   * public:
   *   // Compute f(x).
   *   Spacy::Real operator()(const Spacy::Vector& x) const;
   *
   *   // Compute f'(x) as element of the dual space X*.
   *   Spacy::Vector d1(const Spacy::Vector& x) const;
   *
   *   // Compute f''(x)dx as element of the dual space X*.
   *   Spacy::Vector d2(const Spacy::Vector& x, const Spacy::Vector& dx) const;
   *
   *   // Access f''(x) as mapping f''(x): X->X*.
   *   Spacy::LinearOperator hessian(const Spacy::Vector& x);
   *
   *   // Access underlying domain space X.
   *   const Spacy::VectorSpace& domain() const;
   * };
   * @endcode
   *
   * The member function `hessian(x)` is allowed to return any type that satisfies the LinearOperatorConcept.
   *
   * @see FunctionalConcept, C1FunctionalConcept, C2Functional
   */
  template <class Impl>
  using C2FunctionalConcept =
  std::integral_constant< bool ,
    C1FunctionalConcept<Impl>::value &&
    HasMemFn_d2_Functional<Impl,Vector>::value &&
    HasMemFn_hessian<Impl,Vector>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_C2_FUNCTIONAL_CONCEPT_HH
