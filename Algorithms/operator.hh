#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/member.hpp>

#include "vector.hh"
#include "vectorSpace.hh"
#include "linearSolver.hh"

#include "Util/conceptBase.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_domain) , domain , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_domain_ptr) , domain_ptr , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_range) , range , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_range_ptr) , range_ptr , 0 )

BOOST_TYPE_ERASURE_MEMBER( (has_d1) , d1 , 2 )

BOOST_TYPE_ERASURE_MEMBER( (has_linearization) , linearization , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_solver) , solver , 0 )

namespace Algorithm
{
  using OperatorConcept =
  boost::mpl::vector<
    ConceptBase ,
    OperatorApplyConcept ,
    has_domain<VectorSpace&()> ,
    has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_domain_ptr<VectorSpace*(), const boost::type_erasure::_self> ,
    has_range<VectorSpace&()> ,
    has_range<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_range_ptr<VectorSpace*(), const boost::type_erasure::_self>
  >;

  using LinearOperatorConcept =
  boost::mpl::vector<
    OperatorConcept ,
    has_solver<LinearSolver(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Linear operator class. Plug your implementations in here.
   */
  using LinearOperator = boost::type_erasure::any< boost::mpl::vector< LinearOperatorConcept > >;

  using C1OperatorConcept =
  boost::mpl::vector<
    OperatorConcept ,
    has_d1<Vector(const Vector&,const Vector&), const boost::type_erasure::_self> ,
    has_linearization<LinearOperator(const Vector&), const boost::type_erasure::_self> ,
    has_solver<LinearSolver(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Operator class. Plug your implementations in here.
   */
  using Operator = boost::type_erasure::any< boost::mpl::vector< OperatorConcept > >;

  using C1Operator = boost::type_erasure::any< boost::mpl::vector< C1OperatorConcept > >;


  inline LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
}

#endif // ALGORITHM_OPERATOR_HH
