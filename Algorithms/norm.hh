#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "Util/conceptBase.hh"

#include "vector.hh"

namespace Algorithm
{
  using NormConcept = boost::type_erasure::callable<double(const Vector&), const boost::type_erasure::_self>;

  /**
   * @brief Norm class. Plug your implementations in here.
   */
  using Norm = boost::type_erasure::any< boost::mpl::vector< ConceptBase , NormConcept > >;
}


#endif // ALGORITHM_NORM_HH
