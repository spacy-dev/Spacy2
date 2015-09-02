#ifndef ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH
#define ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  namespace Concepts
  {
    /**
     * \ingroup ConceptGroup
     * \anchor ScalarProductConceptAnchor
     * \brief Concept for scalar products.
     *
     * The minimal signature of a scalar product is:
     * \code
     * // My scalar product.
     * class MyScalarProduct
     * {
     * public:
     *   // Copy constructor.
     *   MyScalarProduct(const MyScalarProduct&);
     *
     *   // Move constructor.
     *   MyScalarProduct(MyScalarProduct&&);
     *
     *   // Compute (x,y).
     *   double operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
     * };
     * \endcode
     *
     * See \ref ScalarProductAnchor "::Spacy::ScalarProduct".
     */
    using ScalarProductConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<double(const boost::type_erasure::any<VectorConcept>&,
                                           const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH
