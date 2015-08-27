#ifndef ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
#define ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @brief Concept for linear solver implementations. Same as CallableOperatorConcept.
     *
     * The minimal signature of a linear solver is:
     * @code
     * // My linear solver representing A^-1.
     * class MyLinearSolver
     * {
     * public:
     *   // Copy constructor.
     *   MyLinearSolver(const MyLinearSolver&);
     *
     *   // Move constructor.
     *   MyLinearSolver(MyLinearSolver&&);
     *
     *   // Compute A^-1(x).
     *   ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const;
     * };
     * @endcode
     *
     * @see ::Algorithm::LinearSolver, CallableOperatorConcept
     */
    using LinearSolverConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>
    >;

    /**
     * @ingroup ConceptGroup
     * @brief Concept for linear solver implementations that monitor positive definiteness.
     *
     * The minimal signature of a general linear solver is:
     * @code
     * // My general linear solver S representing A^-1.
     * class MyGeneralLinearSolver
     * {
     * public:
     *   // Copy constructor.
     *   MyGeneralLinearSolver(const MyGeneralLinearSolver&);
     *
     *   // Move constructor.
     *   MyGeneralLinearSolver(MyGeneralLinearSolver&&);
     *
     *   // Compute A^-1(x).
     *   ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const;
     *
     *   // Check if A is positive definite
     *   bool isPositiveDefinite() const;
     * };
     * @endcode
     *
     * @see ::Algorithm::GeneralLinearSolver
     */
    using GeneralLinearSolverConcept =
    boost::mpl::vector<
      LinearSolverConcept ,
      has_isPositiveDefinite<bool(), const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
