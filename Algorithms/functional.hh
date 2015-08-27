#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/functionalConcept.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  /**
   * @ingroup VHatGroup
   * @brief Functional. Can store objects that satisfy the requirements of FunctionalConcept.
   * @see Concepts::FunctionalConcept
   */
  using Functional = boost::type_erasure::any<Concepts::FunctionalConcept>;

  /**
   * @ingroup VHatGroup
   * @brief Differentiable functional. Can store objects that satisfy the requirements of C1FunctionalConcept.
   * @see Concepts::C1FunctionalConcept
   */
  using C1Functional = boost::type_erasure::any<Concepts::C1FunctionalConcept>;

  /**
   * @ingroup VHatGroup
   * @brief Twice differentiable functional. Can store objects that satisfy the requirements of C2FunctionalConcept.
   * @see Concepts::C2FunctionalConcept
   */
  using C2Functional = boost::type_erasure::any<Concepts::C2FunctionalConcept>;
}

#endif // ALGORITHM_FUNCTIONAL_HH
