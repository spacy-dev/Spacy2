#ifndef ALGORITHM_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH
#define ALGORITHM_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>

#include "Algorithms/Util/Concepts/conceptBase.hh"
#include "Algorithms/Util/Concepts/vectorConcept.hh"

namespace Algorithm
{
  namespace Concepts
  {
    namespace Newton
    {
      /**
       * @ingroup NewtonConceptGroup
       * @anchor TerminationCriterionConceptAnchor
       * @brief Concept for termination criteria for Newton's method.
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
       *   bool operator()(double nu, const Vector& x, const Vector& dx) const;
       * };
       * @endcode
       *
       * @see @ref Newton_TerminationCriterionAnchor "Newton::TerminationCriterion", @ref VectorAnchor "Vector"
       */
      using TerminationCriterionConcept =
      boost::mpl::vector<
        ::Algorithm::Concepts::ConceptBase ,
        boost::type_erasure::callable<bool(double, const boost::type_erasure::any< ::Algorithm::Concepts::VectorConcept >&,
                                           const boost::type_erasure::any< ::Algorithm::Concepts::VectorConcept >&),
                                      const boost::type_erasure::_self>
      >;
    }
  }
}

#endif // ALGORITHM_CONCEPTS_NEWTON_TERMINATION_CRITERION_CONCEPT_HH
