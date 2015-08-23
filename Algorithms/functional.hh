#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/functionalConcept.hh"

namespace Algorithm
{
  /**
   * @brief Functional. Can store objects that satisfy the requirements of FunctionalConcept.
   * @see FunctionalConcept
   */
  using Functional = boost::type_erasure::any<FunctionalConcept>;

  /**
   * @brief Differentiable functional. Can store objects that satisfy the requirements of C1FunctionalConcept.
   * @see C1FunctionalConcept
   */
  using C1Functional = boost::type_erasure::any<C1FunctionalConcept>;

  /**
   * @brief Twice differentiable functional. Can store objects that satisfy the requirements of C2FunctionalConcept.
   * @see C2FunctionalConcept
   */
  using C2Functional = boost::type_erasure::any<C2FunctionalConcept>;
}

#endif // ALGORITHM_FUNCTIONAL_HH
