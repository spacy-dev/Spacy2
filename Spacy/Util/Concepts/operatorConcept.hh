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

  template <class Impl>
  using OperatorConcept =
  std::integral_constant< bool ,
    std::is_copy_constructible<Impl>::value &&
    std::is_copy_assignable<Impl>::value &&
    HasMemOp_callable<Impl,Vector,Vector>::value &&
    HasMemFn_domain<Impl>::value &&
    HasMemFn_range<Impl>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_OPERATOR_CONCEPT_HH
