#ifndef SPACY_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH
#define SPACY_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>

#include "Spacy/Util/Concepts/conceptBase.hh"
#include "Spacy/Util/Concepts/vectorConcept.hh"

namespace Spacy
{
  /// @cond
  class DampingFactor;
  class Vector;
  /// @endcond

  namespace Concepts
  {
    namespace Newton
    {
      /**
       * @ingroup NewtonConceptGroup
       * @anchor Newton_TerminationCriterionConceptAnchor
       * @brief Concept for termination criteria for %Newton's method.
       *
       * The minimal signature of a termination criterion is:
       * @code
       * // My termination criterion.
       * class MyTerminationCriterion
       * {
       * public:
       *   // Copy constructor.
       *   MyTerminationCriterion(const MyTerminationCriterion&);
       *
       *   // Move constructor.
       *   MyTerminationCriterion(MyTerminationCriterion&&);
       *
       *   // Check if termination criterion is satisfied.
       *   bool operator()(DampingFactor nu, const Vector& x, const Vector& dx) const;
       * };
       * @endcode
       *
       * @see @ref Newton_TerminationCriterionAnchor "Newton::TerminationCriterion", @ref VectorAnchor "Vector"
       */
      using TerminationCriterionConcept =
      boost::mpl::vector<
        ::Spacy::Concepts::ConceptBase ,
        boost::type_erasure::callable<bool(DampingFactor,
                                           const Vector&,
                                           const Vector&),
                                      const boost::type_erasure::_self>
//      boost::type_erasure::callable<bool(DampingFactor,
//                                         const boost::type_erasure::any< ::Spacy::Concepts::VectorConcept >&,
//                                         const boost::type_erasure::any< ::Spacy::Concepts::VectorConcept >&),
//                                    const boost::type_erasure::_self>
      >;
    }
  }
}

#endif // SPACY_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH
