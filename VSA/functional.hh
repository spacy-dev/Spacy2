#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/functionalConcept.hh"

#include "vectorSpace.hh"

namespace VSA
{
  /**
   * @ingroup VSpaceGroup
   * @anchor FunctionalAnchor
   * @brief Functional. Can store objects that satisfy the requirements of \ref FunctionalConceptAnchor "FunctionalConcept".
   */
  using Functional = boost::type_erasure::any<Concepts::FunctionalConcept>;

  /**
   * @ingroup VSpaceGroup
   * @anchor C1FunctionalAnchor
   * @brief Differentiable functional. Can store objects that satisfy the requirements of \ref C1FunctionalConceptAnchor "C1FunctionalConcept".
   */
  using C1Functional = boost::type_erasure::any<Concepts::C1FunctionalConcept>;

  /**
   * @ingroup VSpaceGroup
   * @anchor C2FunctionalAnchor
   * @brief Twice differentiable functional. Can store objects that satisfy the requirements of \ref C2FunctionalConceptAnchor "C2FunctionalConcept".
   */
  using C2Functional = boost::type_erasure::any<Concepts::C2FunctionalConcept>;
}

#endif // ALGORITHM_FUNCTIONAL_HH
