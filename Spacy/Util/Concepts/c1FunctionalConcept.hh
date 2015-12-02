// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_C1_FUNCTIONAL_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_C1_FUNCTIONAL_CONCEPT_HH

#include "Spacy/Util/Concepts/functionalConcept.hh"

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
