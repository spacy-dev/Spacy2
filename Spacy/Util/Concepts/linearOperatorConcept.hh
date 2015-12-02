// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_LINEAR_OPERATOR_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_LINEAR_OPERATOR_CONCEPT_HH

#include "operatorConcept.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  template <class Impl>
  using LinearOperatorConcept =
  std::integral_constant< bool ,
    VectorConcept<Impl>::value &&
    OperatorConcept<Impl>::value &&
    HasMemFn_solver<Impl>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_LINEAR_OPERATOR_CONCEPT_HH
