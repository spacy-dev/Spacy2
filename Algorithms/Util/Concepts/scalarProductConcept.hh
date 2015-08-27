#ifndef ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH
#define ALGORITHM_SCALAR_PRODUCT_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Algorithm
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
     *   double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
     * };
     * \endcode
     *
     * See \ref ScalarProductAnchor "::Algorithm::ScalarProduct".
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
