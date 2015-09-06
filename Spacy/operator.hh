#ifndef SPACY_OPERATOR_HH
#define SPACY_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/operatorConcept.hh"
#include "Util/Concepts/vectorConcept.hh"

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
  const boost::type_erasure::any<Concepts::LinearSolverConcept>& operator^(const LinearOperator& A, int k);

  /**
   * \ingroup SpacyGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  boost::type_erasure::any<Concepts::LinearSolverConcept> operator^(LinearOperator&& A, int k);

  /**
   * @ingroup SpacyGroup
   * @brief For an operator \f$ A: X\to Y \f$, compute \f$A'\f$ at \f$x\in X\f$ as linear operator \f$ A'(x): X \to Y \f$.
   *
   * Equivalent to calling A.linearization(x).
   *
   * @param A differentiable operator
   * @param x point of linearization
   * @return \f$A'(x)\f$, i.e. A.linearization(x).
   * @see @ref C1OperatorAnchor "C1Operator", @ref LinearOperatorAnchor "LinearOperator"
   */
  LinearOperator d1(const C1Operator& A, const boost::type_erasure::any< Concepts::VectorConcept >& x);
}

#endif // SPACY_OPERATOR_HH
