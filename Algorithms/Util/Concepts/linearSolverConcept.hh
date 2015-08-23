#ifndef ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
#define ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  using LinearSolverConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                                  const boost::type_erasure::_self>
  >;

  using GeneralLinearSolverConcept =
  boost::mpl::vector<
    LinearSolverConcept ,
    has_isPositiveDefinite<bool(), const boost::type_erasure::_self>
  >;
}

#endif // ALGORITHM_CONCEPTS_LINEAR_SOLVER_CONCEPT_HH
