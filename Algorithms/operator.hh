#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/operatorConcept.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  /**
   * \ingroup VHatGroup
   * \anchor CallableOperatorAnchor
   * \brief Simplest possible operator class. Can store objects that satisfy the requirements of \ref CallableOperatorConceptAnchor "CallableOperatorConcept".
   *
   * Use this if only application of an operator is required.
   */
  using CallableOperator = boost::type_erasure::any< Concepts::CallableOperatorConcept >;

  /**
   * \ingroup VHatGroup
   * \anchor LinearOperatorAnchor
   * \brief Linear operator class. Can store objects that satisfy the requirements of \ref LinearOperatorConceptAnchor "LinearOperatorConcept".
   */
  using LinearOperator = boost::type_erasure::any< Concepts::LinearOperatorConcept >;

  /**
   * \ingroup VHatGroup
   * \anchor OperatorAnchor
   * \brief Operator class. Can store objects that satisfy the requirements of \ref OperatorConceptAnchor "OperatorConcept".
   */
  using Operator = boost::type_erasure::any< Concepts::OperatorConcept >;

  /**
   * \ingroup VHatGroup
   * \anchor C1OperatorAnchor
   * \brief Differentiable operator class. Can store objects that satisfy the requirements of \ref C1OperatorConceptAnchor "C1OperatorConcept".
   */
  using C1Operator = boost::type_erasure::any< Concepts::C1OperatorConcept >;

  /**
   * \ingroup VHatGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  inline const auto& operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
}

#endif // ALGORITHM_OPERATOR_HH
