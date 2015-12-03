#ifndef SPACY_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH
#define SPACY_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace CG
  {
    /**
     * @ingroup ConceptGroup
     * @anchor CG_TerminationCriterionConceptAnchor
     * @brief Concept for termination criteria for conjugate gradient methods.
     *
     * @code
     * class MyTerminationCriterion
     * {
     * public:
     *   // Check if termination criterion is satisfied.
     *   bool operator()() const;
     *
     *   // Clear internal storage for new cg run.
     *   void clear();
     *
     *   // Provide algorithmic quantities required to evaluate termination criteria.
     *   void update(double alpha, double qAq, double qPq, double rPINVr)
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
    template <class T>
    using TerminationCriterionConcept =
    std::integral_constant< bool ,
      HasMemOp_variadicCallable<bool>::template apply<T>::value &&
      HasMemFn_clear<T>::value &&
      HasMemFn_update<T>::value &&
      HasMemFn_vanishingStep<T>::value &&
      HasMemFn_minimalDecreaseAchieved<T>::value &&
      HasMemFn_setEps<T>::value &&
      HasMemFn_setAbsoluteAccuracy<T>::value &&
      HasMemFn_setRelativeAccuracy<T>::value &&
      HasMemFn_setMinimalAccuracy<T>::value
    >;
  }
}

#endif // SPACY_CONCEPTS_CG_TERMINATION_CRITERION_CONCEPT_HH
