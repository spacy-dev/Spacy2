#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/operatorConcept.hh"
#include "Util/Concepts/vectorConcept.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include "vectorSpace.hh"

namespace Spacy
{
  /**
   * \ingroup SpacyGroup
   * \anchor CallableOperatorAnchor
   * \brief Simplest possible operator class. Can store objects that satisfy the requirements of \ref CallableOperatorConceptAnchor "CallableOperatorConcept".
   *
   * Use this if only application of an operator is required.
   */
  using CallableOperator = boost::type_erasure::any< Concepts::CallableOperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor LinearOperatorAnchor
   * \brief Linear operator class. Can store objects that satisfy the requirements of \ref LinearOperatorConceptAnchor "LinearOperatorConcept".
   */
  using LinearOperator = boost::type_erasure::any< Concepts::LinearOperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor OperatorAnchor
   * \brief Operator class. Can store objects that satisfy the requirements of \ref OperatorConceptAnchor "OperatorConcept".
   */
  using Operator = boost::type_erasure::any< Concepts::OperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \anchor C1OperatorAnchor
   * \brief Differentiable operator class. Can store objects that satisfy the requirements of \ref C1OperatorConceptAnchor "C1OperatorConcept".
   */
  using C1Operator = boost::type_erasure::any< Concepts::C1OperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  inline const auto& operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  /**
   * \ingroup SpacyGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  inline const auto& operator^(LinearOperator&& A, int k)
  {
    if( k == -1 ) return std::move(A.solver());
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  /**
   * @ingroup SpacyGroup
   * @brief Linearize \f$A\f$ at \f$x\f$.
   * @param A differentiable operator
   * @param x point of linearization
   * @return LinearizedOperator(A,x,solver)
   */
  inline auto linearize(const C1Operator& A, const boost::type_erasure::any< Concepts::VectorConcept >& x)
  {
    return A.linearization(x);
  }
}

#endif // ALGORITHM_OPERATOR_HH
