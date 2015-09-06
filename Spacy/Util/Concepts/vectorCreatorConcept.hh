#ifndef SPACY_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH
#define SPACY_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Spacy
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
     * @see VectorSpace, \ref VectorCreatorAnchor "VectorCreator".
     */
    using VectorCreatorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const VectorSpace*), const boost::type_erasure::_self>
    >;
  }
}

#endif // SPACY_CONCEPTS_VECTOR_CREATOR_CONCEPT_HH
