#ifndef ALGORITHM_CONCEPTS_CONCEPT_BASE_HH
#define ALGORITHM_CONCEPTS_CONCEPT_BASE_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>

namespace Spacy
{
  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @brief Base concept.
     *
     * Requires copy-constructability, provides run-time type information and some convenience functions with boost::type_erasure::relaxed.
     */
    using ConceptBase =
    boost::mpl::vector<
      // copy
      boost::type_erasure::copy_constructible<> ,
      // run-time type information for cast_ref
      boost::type_erasure::typeid_<> ,
      boost::type_erasure::relaxed
    >;
  }
}
#endif // ALGORITHM_CONCEPTS_CONCEPT_BASE_HH

