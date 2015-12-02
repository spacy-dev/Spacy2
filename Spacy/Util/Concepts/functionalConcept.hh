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

  template <class Impl>
  using FunctionalConcept =
  std::integral_constant< bool ,
    std::is_copy_constructible<Impl>::value &&
    std::is_copy_assignable<Impl>::value &&
    HasMemOp_callable<Impl,Vector,Real>::value &&
    HasMemFn_domain<Impl>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_FUNCTIONAL_CONCEPT_HH
