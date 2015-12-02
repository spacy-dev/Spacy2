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

  template <class Impl>
  using C2FunctionalConcept =
  std::integral_constant< bool ,
    C1FunctionalConcept<Impl>::value &&
    HasMemFn_d2_Functional<Impl,Vector>::value &&
    HasMemFn_hessian<Impl,Vector>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_C2_FUNCTIONAL_CONCEPT_HH
