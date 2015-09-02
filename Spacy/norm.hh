#ifndef ALGORITHM_NORM_HH
#define ALGORITHM_NORM_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/normConcept.hh"

namespace Spacy
{
  /**
   * \ingroup VSpaceGroup
   * \anchor NormAnchor
   * \brief Norm class.  Can store objects that satisfy the requirements of \ref NormConceptAnchor "NormConcept".
   */
  using Norm = boost::type_erasure::any< Concepts::NormConcept >;
}


#endif // ALGORITHM_NORM_HH
