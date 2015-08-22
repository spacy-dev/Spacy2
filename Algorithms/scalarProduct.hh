#ifndef ALGORITHM_SCALARPRODUCT_HH
#define ALGORITHM_SCALARPRODUCT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "Util/conceptBase.hh"

#include "vector.hh"

namespace Algorithm
{
  using ScalarProductConcept = boost::type_erasure::callable<double(const Vector&,const Vector&), const boost::type_erasure::_self>;

  /**
   * @brief Norm class. Plug your implementations in here.
   */
  using ScalarProduct = boost::type_erasure::any< boost::mpl::vector< ConceptBase , ScalarProductConcept > >;
}
#endif // ALGORITHMS_SCALARPRODUCT_HH
