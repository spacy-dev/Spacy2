#ifndef SPACY_NORM_HH
#define SPACY_NORM_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/normConcept.hh"

namespace Spacy
{
  /**
   * \ingroup SpacyGroup
   * \anchor NormAnchor
   * \brief Norm class.  Can store objects that satisfy the requirements of \ref NormConceptAnchor "NormConcept".
   */
  using Norm = boost::type_erasure::any< Concepts::NormConcept >;
}


#endif // SPACY_NORM_HH
