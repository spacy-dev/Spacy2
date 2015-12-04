// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_OPERATOR_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_OPERATOR_CONCEPT_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * @ingroup ConceptGroup
   * @anchor OperatorConceptAnchor
   *
   * @code
   * // A: X->Y.
   * class MyOperator
   * {
   * public:
   *   // Compute A(x).
   *   Vector operator()(const Vector& x) const;
   *
   *   // Access domain space X.
   *   const VectorSpace& domain() const;
   *
   *   // Access range space Y.
   *   const VectorSpace& range() const;
   * };
   * @endcode
   * @see Operator
   */
  template <class Impl>
  using OperatorConcept =
  std::integral_constant< bool ,
    HasMemOp_callable<Impl,Vector,Vector>::value &&
    HasMemFn_domain<Impl>::value &&
    HasMemFn_range<Impl>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_OPERATOR_CONCEPT_HH
