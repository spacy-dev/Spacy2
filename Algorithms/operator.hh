#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/operatorConcept.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  /**
   * @brief Simplest possible operator class. Can store objects that satisfy the requirements of CallableOperatorConcept.
   *
   * Use this if only application of an operator is required.
   *
   * @see CallableOperatorConcept
   */
  using CallableOperator = boost::type_erasure::any< CallableOperatorConcept >;

  /**
   * @brief Linear operator class. Can store objects that satisfy the requirements of LinearOperatorConcept.
   * @see LinearOperatorConcept
   */
  using LinearOperator = boost::type_erasure::any< LinearOperatorConcept >;

  /**
   * @brief Operator class. Can store objects that satisfy the requirements of OperatorConcept.
   * @see OperatorConcept
   */
  using Operator = boost::type_erasure::any< OperatorConcept >;

  /**
   * @brief Differentiable perator class. Can store objects that satisfy the requirements of C1OperatorConcept.
   * @see C1OperatorConcept
   */
  using C1Operator = boost::type_erasure::any< C1OperatorConcept >;

  /**
   * @brief Access solver via A^-1. Throws for k!=-1.
   */
  inline auto operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
}

#endif // ALGORITHM_OPERATOR_HH
