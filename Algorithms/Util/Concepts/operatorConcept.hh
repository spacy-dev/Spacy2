#ifndef ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH
#define ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "linearSolverConcept.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Concept defining the simplest possible operator, only providing Vector operator()(const Vector&).
   * @see ::Algorithm::Operator
   */
  using CallableOperatorConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                                  const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept defining general operators.
   * @see ::Algorithm::Operator
   */
  using OperatorConcept =
  boost::mpl::vector<
    CallableOperatorConcept ,
    has_domain<VectorSpace&()> ,
    has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_domain_ptr<VectorSpace*(), const boost::type_erasure::_self> ,
    has_range<VectorSpace&()> ,
    has_range<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_range_ptr<VectorSpace*(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept defining linear operators.
   * @see ::Algorithm::LinearOperator
   */
  using LinearOperatorConcept =
  boost::mpl::vector<
    OperatorConcept ,
    has_solver<const boost::type_erasure::any<LinearSolverConcept>&(),
               const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept defining differentiable operators.
   * @see ::Algorithm::C1Operator
   */
  using C1OperatorConcept =
  boost::mpl::vector<
    OperatorConcept ,
    has_d1<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
           const boost::type_erasure::_self> ,
    has_linearization<boost::type_erasure::any<LinearOperatorConcept>(const boost::type_erasure::any<VectorConcept>&),
                      const boost::type_erasure::_self>
  >;
}

#endif // ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH
