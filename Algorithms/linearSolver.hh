#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/linearSolverConcept.hh"

namespace Algorithm
{
  /**
   * \brief Linear solver class. Same as CallableOperator.
   * \anchor LinearSolverAnchor
   * See \ref LinearSolverConceptAnchor "LinearSolverConcept".
   */
  using LinearSolver = boost::type_erasure::any< Concepts::LinearSolverConcept >;

  /**
   * \brief General linear solver class. Additionally monitors if the underlying operator is positive definite.
   * \anchor IndefiniteLinearSolverAnchor
   * See \ref IndefiniteLinearSolverConceptAnchor "IndefiniteLinearSolverConcept".
   */
  using IndefiniteLinearSolver = boost::type_erasure::any< Concepts::IndefiniteLinearSolverConcept >;
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
