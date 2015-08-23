#ifndef ALGORITHM_NORM_CONCEPT_HH
#define ALGORITHM_NORM_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  using NormConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<double(const boost::type_erasure::any< VectorConcept >&), const boost::type_erasure::_self>
  >;
}

#endif // ALGORITHM_NORM_CONCEPT
