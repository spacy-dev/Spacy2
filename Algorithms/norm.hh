#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/normConcept.hh"

namespace Algorithm
{
  /**
   * @brief Norm class.  Can store objects that satisfy the requirements of NormConcept.
   * @see NormConcept
   */
  using Norm = boost::type_erasure::any< NormConcept >;
}


#endif // ALGORITHM_NORM_HH
