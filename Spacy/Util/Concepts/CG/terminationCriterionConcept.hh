#ifndef ALGORITHM_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH
#define ALGORITHM_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/member.hpp>

#include "Spacy/Util/Concepts/conceptBase.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_clear) , clear , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_update) , update , 4 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_vanishingStep) , vanishingStep , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_minimalDecreaseAchieved) , minimalDecreaseAchieved , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_setEps) , setEps , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_setRelativeAccuracy) , setRelativeAccuracy , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_setAbsoluteAccuracy) , setAbsoluteAccuracy , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_cg_terminate_setMinimalAccuracy) , setMinimalAccuracy , 1 )


namespace Spacy
{
  namespace Concepts
  {
    namespace CG
    {
      /**
       * @ingroup CGConceptGroup
       * @anchor CG_TerminationCriterionConceptAnchor
       * @brief Concept for termination criteria for conjugate gradient methods.
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
       *   bool operator()() const;
       *
       *   // Clear internal storage for new cg run.
       *   void clear();
       *
       *   // Provide algorithmic quantities required to evaluate termination criteria.
       *   void update(double stepLength, double qAq, double qPq, double rPINVr)
       *
       *   // Checks if the step length of the computed step is below the maximal attainable accuracy.
       *   bool vanishingStep() const;
       *
       *   // Checks if at least the minimal required decrease is satisfied. This is required for the
       *   // truncated regularized conjugate gradient method (TRCG).
       *   bool minimalDecreaseAchieved() const;
       *
       *   // Set maximal attainable accuracy
       *   void setEps(double);
       *
       *   // Set relative accuracy.
       *   void setRelativeAccuracy(double);
       *
       *   // Set absolute accuracy.
       *   void setAbsoluteAccuracy(double);
       *
       *   // Set minimal accuracy. Only required for truncated regularized conjugate gradients (TRCG).
       *   void setMinimalAccuracy(double);
       * };
       * @endcode
       *
       * @see @ref CG_TerminationCriterionAnchor "CG::TerminationCriterion"
       */
      using TerminationCriterionConcept = boost::mpl::vector<
        ConceptBase ,
        boost::type_erasure::callable<bool(), const boost::type_erasure::_self> ,
        has_cg_terminate_clear<void()> ,
        has_cg_terminate_update<void(double,double,double,double)> ,
        has_cg_terminate_vanishingStep<bool(), const boost::type_erasure::_self> ,
        has_cg_terminate_minimalDecreaseAchieved<bool(), const boost::type_erasure::_self> ,
        has_cg_terminate_setEps<void(double)> ,
        has_cg_terminate_setRelativeAccuracy<void(double)> ,
        has_cg_terminate_setAbsoluteAccuracy<void(double)> ,
        has_cg_terminate_setMinimalAccuracy<void(double)>
      >;
    }
  }
}

#endif // ALGORITHM_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH
