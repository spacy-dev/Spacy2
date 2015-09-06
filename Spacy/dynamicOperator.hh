#ifndef SPACY_DYNAMIC_OPERATOR_HH
#define SPACY_DYNAMIC_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/dynamicOperatorConcept.hh"
#include "Util/Concepts/vectorConcept.hh"

#include "vectorSpace.hh"

namespace Spacy
{
  /**
   * \ingroup SpacyGroup
   * \anchor DynamicCallableOperatorAnchor
   * \brief Simplest possible time-dependent operator. Can store objects that satisfy the requirements of \ref DynamicCallableOperatorConceptAnchor "DynamicCallableOperatorConcept".
   *
   * Use this if only application of an operator is required.
   */
  using DynamicCallableOperator = boost::type_erasure::any< Concepts::DynamicCallableOperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor DynamicLinearOperatorAnchor
   * \brief Time-dependent linear operator class. Can store objects that satisfy the requirements of \ref DynamicLinearOperatorConceptAnchor "DynamicLinearOperatorConcept".
   */
  using DynamicLinearOperator = boost::type_erasure::any< Concepts::DynamicLinearOperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor DynamicOperatorAnchor
   * \brief Time-dependent operator. Can store objects that satisfy the requirements of \ref DynamicOperatorConceptAnchor "DynamicOperatorConcept".
   */
  using DynamicOperator = boost::type_erasure::any< Concepts::DynamicOperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor DynamicC1OperatorAnchor
   * \brief Time-dependent differentiable operator. Can store objects that satisfy the requirements of \ref DynamicC1OperatorConceptAnchor "DynamicC1OperatorConcept".
   */
  using DynamicC1Operator = boost::type_erasure::any< Concepts::DynamicC1OperatorConcept >;
}

#endif // SPACY_DYNAMIC_OPERATOR_HH

