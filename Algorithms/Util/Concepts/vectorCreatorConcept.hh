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
     * @ingroup ConceptGroup
     * @anchor VectorCreatorConceptAnchor
     * @brief Vector creator concept.
     * @see \ref VectorSpaceAnchor "VectorSpace", \ref VectorCreatorAnchor "VectorCreator".
     */
    using VectorCreatorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const VectorSpace*), const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH