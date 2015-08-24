#ifndef ALGORITHM_CONCEPTS_CONCEPT_BASE_HH
#define ALGORITHM_CONCEPTS_CONCEPT_BASE_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>

namespace Algorithm
{
  using ConceptBase =
  boost::mpl::vector<
    // copy
    boost::type_erasure::copy_constructible<> ,
    // run-time type information for cast_ref
    boost::type_erasure::typeid_<> ,
    boost::type_erasure::relaxed
  >;

  using MovableConceptBase =
  boost::mpl::vector<
    // move
    boost::type_erasure::constructible<boost::type_erasure::_self(boost::type_erasure::_self&&)> ,
    // run-time type information for cast_ref
    boost::type_erasure::typeid_<> ,
    boost::type_erasure::relaxed
  >;
}
#endif // ALGORITHM_CONCEPTS_CONCEPT_BASE_HH

