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

  template <class Impl>
  using C1OperatorConcept =
  std::integral_constant< bool ,
    OperatorConcept<Impl>::value &&
    HasMemFn_d1_Operator<Impl,Vector>::value &&
    HasMemFn_linearization<Impl,Vector>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_C1_OPERATOR_CONCEPT_HH
