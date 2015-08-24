#ifndef ALGORITHM_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH
#define ALGORITHM_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Concepts
  {
    /**
     * @brief Vector creator concept.
     * @see VectorCreator, VectorSpace
     */
    using VectorCreatorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const VectorSpace*), const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH
