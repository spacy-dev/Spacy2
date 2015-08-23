#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/linearSolverConcept.hh"

namespace Algorithm
{
  /**
   * @brief Linear solver class. Same as CallableOperator.
   *
   * @see LinearSolverConcept
   */
  using LinearSolver = boost::type_erasure::any< LinearSolverConcept >;

  /**
   * @brief General linear solver class. Additionally monitors if the underlying operator is positive definite.
   *
   * @see GeneralLinearSolverConcept
   */
  using GeneralLinearSolver = boost::type_erasure::any< GeneralLinearSolverConcept >;
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
