#ifndef SPACY_SCALARPRODUCT_HH
#define SPACY_SCALARPRODUCT_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/scalarProductConcept.hh"

namespace Spacy
{
  /**
   * \ingroup SpacyGroup
   * \anchor ScalarProductAnchor
   * \brief Scalar product class.  Can store objects that satisfy the requirements of \ref ScalarProductConceptAnchor "ScalarProductConcept".
   */
  using ScalarProduct = boost::type_erasure::any< Concepts::ScalarProductConcept >;
}
#endif // SPACYS_SCALARPRODUCT_HH
