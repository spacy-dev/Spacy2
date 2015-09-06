#ifndef SPACY_NORM_CONCEPT_HH
#define SPACY_NORM_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  class Real;
  /// @endcond
  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @anchor NormConceptAnchor
     * @brief Concept for norm implementations.
     *
     * The minimal signature of a norm is:
     * @code
     * // My norm.
     * class MyNorm
     * {
     * public:
     *   // Copy constructor.
     *   MyNorm(const MyNorm&);
     *
     *   // Move constructor.
     *   MyNorm(MyNorm&&);
     *
     *   // Compute ||x||.
     *   Real operator()(const ::Spacy::Vector& x) const;
     * };
     * @endcode
     *
     * See @ref NormAnchor "::Spacy::Norm".
     */
    using NormConcept =
    boost::mpl::vector<
      ConceptBase ,
    boost::type_erasure::callable<Real(const Vector&), const boost::type_erasure::_self>
//    boost::type_erasure::callable<Real(const boost::type_erasure::any< VectorConcept >&), const boost::type_erasure::_self>
    >;
  }
}

#endif // SPACY_NORM_CONCEPT
