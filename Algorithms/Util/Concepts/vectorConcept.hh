#ifndef ALGORITHM_VECTOR_CONCEPT_HH
#define ALGORITHM_VECTOR_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/operators.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"

namespace Algorithm
{
  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @brief Concept for vector implementations.
     * @see ::Algorithm::Vector
     */
    using VectorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::assignable<> ,
      boost::type_erasure::multiply_assignable< boost::type_erasure::_self , double > ,
      boost::type_erasure::add_assignable<> ,
      boost::type_erasure::subtract_assignable<> ,
      boost::type_erasure::negatable<> ,
      boost::type_erasure::equality_comparable<> ,
      boost::type_erasure::callable<double(const boost::type_erasure::_self&),
                                    const boost::type_erasure::_self> ,
      has_spaceIndex<unsigned(),
                     const boost::type_erasure::_self> ,
      has_isAdmissible<bool(),
                       const boost::type_erasure::_self> ,
      has_norm<double(),
               const boost::type_erasure::_self> ,
      has_scalarProduct<double(const boost::type_erasure::_self&),
                        const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_VECTOR_CONCEPT_HH
