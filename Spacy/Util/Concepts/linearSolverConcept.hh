#ifndef ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
#define ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @anchor LinearSolverConceptAnchor
     * @brief Concept for linear solver implementations. Same as @ref CallableOperatorConceptAnchor "CallableOperatorConcept".
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
     *   ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;
     * };
     * @endcode
     *
     * See @ref LinearSolverAnchor "::Spacy::LinearSolver", @ref CallableOperatorConceptAnchor "CallableOperatorConcept"
     */
    using LinearSolverConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>
    >;

    /**
     * @ingroup ConceptGroup
     * @anchor IndefiniteLinearSolverConceptAnchor
     * @brief Concept for linear solver implementations that monitor positive definiteness.
     *
     * The minimal signature of a general linear solver is:
     * @code
     * // My general linear solver S representing A^-1.
     * class MyIndefiniteLinearSolver
     * {
     * public:
     *   // Copy constructor.
     *   MyIndefiniteLinearSolver(const MyIndefiniteLinearSolver&);
     *
     *   // Move constructor.
     *   MyIndefiniteLinearSolver(MyIndefiniteLinearSolver&&);
     *
     *   // Compute A^-1(x).
     *   ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;
     *
     *   // Check if A is positive definite
     *   bool isPositiveDefinite() const;
     * };
     * @endcode
     *
     * See @ref IndefiniteLinearSolverAnchor "::Spacy::IndefiniteLinearSolver".
     */
    using IndefiniteLinearSolverConcept =
    boost::mpl::vector<
      LinearSolverConcept ,
      has_isPositiveDefinite<bool(), const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
