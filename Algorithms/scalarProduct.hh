#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/scalarProductConcept.hh"

namespace Algorithm
{
  /**
   * @brief Scalar product class.  Can store objects that satisfy the requirements of ScalarProductConcept.
   * @see Concepts::ScalarProductConcept
   */
  using ScalarProduct = boost::type_erasure::any< Concepts::ScalarProductConcept >;
}
#endif // ALGORITHMS_SCALARPRODUCT_HH
