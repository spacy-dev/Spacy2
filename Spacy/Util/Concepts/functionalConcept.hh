// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_FUNCTIONAL_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_FUNCTIONAL_CONCEPT_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  /**
   * @ingroup ConceptGroup
   * @anchor FunctionalConceptGroup
   * @brief Concept for functionals in %Spacy.
   *
   * @code
   * // f: X->R.
   * class MyFunctional
   * {
   * public:
   *   // Compute f(x).
   *   Real operator()(const ::Spacy::Vector& x) const;
   *
   *   // Access underlying domain space X.
   *   const VectorSpace& domain() const;
   * };
   *
   * @endcode
   * @see Functional
   */
  template <class Impl>
  using FunctionalConcept =
  std::integral_constant< bool ,
    HasMemOp_callable<Impl,Vector,Real>::value &&
    HasMemFn_domain<Impl>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_FUNCTIONAL_CONCEPT_HH
