#ifndef ALGORITHM_LINEAR_SOLVER_HH
#define ALGORITHM_LINEAR_SOLVER_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/member.hpp>

#include "Util/conceptBase.hh"

#include "vector.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_isPositiveDefinite) , isPositiveDefinite , 0 )

namespace Algorithm
{
  using OperatorApplyConcept = boost::type_erasure::callable<Vector(const Vector&), const boost::type_erasure::_self>;

  /**
   * @brief Norm class. Plug your implementations in here.
   */
  using CallableOperator = boost::type_erasure::any< boost::mpl::vector< ConceptBase , OperatorApplyConcept > >;
  using LinearSolver = CallableOperator;

  using GeneralLinearSolver =
  boost::type_erasure::any<
    boost::mpl::vector<
      ConceptBase ,
      OperatorApplyConcept ,
      has_isPositiveDefinite<bool(), const boost::type_erasure::_self>
    >
  >;
}

#endif // ALGORITHM_LINEAR_SOLVER_HH
