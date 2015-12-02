// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH
#define SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  /// Concept for vector implementations.
  /// Evaluates to std::true_type if Impl satisfies VectorConcept, else to std::false_type.
  template <class Impl>
  using VectorConcept =
  std::integral_constant< bool ,
// The following line does not compile with gcc-5.2.0
//    HasMemOp_callable<Impl,Impl,Real>::value &&
    std::is_copy_constructible<Impl>::value &&
    std::is_copy_assignable<Impl>::value &&
    HasMemOp_add<Impl>::value &&
    HasMemOp_subtract<Impl>::value &&
    HasMemOp_multiply<Impl>::value &&
    HasMemOp_negate<Impl>::value &&
    HasMemFn_space<Impl>::value &&
    IsVoid< Impl >::template apply<TryMemFn_toFile>::value
  >;
}

#endif // SPACY_UTIL_CONCEPTS_VECTOR_CONCEPT_HH
