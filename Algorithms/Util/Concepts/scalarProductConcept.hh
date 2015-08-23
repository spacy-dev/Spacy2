#ifndef ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH
#define ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  /**
   * @brief Concept for scalar products.
   * @see ScalarProduct
   */
  using ScalarProductConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<double(const boost::type_erasure::any<VectorConcept>&,
                                         const boost::type_erasure::any<VectorConcept>&),
                                  const boost::type_erasure::_self>
  >;
}

#endif // ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH
