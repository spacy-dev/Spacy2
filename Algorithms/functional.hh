#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/member.hpp>

#include "Util/conceptBase.hh"

#include "vector.hh"
#include "vectorSpace.hh"
#include "operator.hh"

BOOST_TYPE_ERASURE_MEMBER( (has_d1_dual) , d1 , 1 )

BOOST_TYPE_ERASURE_MEMBER( (has_d2) , d2 , 3 )

BOOST_TYPE_ERASURE_MEMBER( (has_d2_dual) , d2 , 2 )

BOOST_TYPE_ERASURE_MEMBER( (has_hessian) , hessian , 1 )

namespace Algorithm
{
  /**
   * @brief Concept for functionals.
   */
  using FunctionalConcept =
  boost::mpl::vector<
    boost::type_erasure::callable<double(const Vector&), const boost::type_erasure::_self> ,
    has_domain<VectorSpace&()> ,
    has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_domain_ptr<VectorSpace*(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept for differentiable functionals.
   */
  using C1FunctionalConcept =
  boost::mpl::vector<
    FunctionalConcept ,
    has_d1<double(const Vector&, const Vector&), const boost::type_erasure::_self> ,
    has_d1_dual<Vector(const Vector&), const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept for twice differentiable functionals.
   */
  using C2FunctionalConcept =
  boost::mpl::vector<
    C1FunctionalConcept ,
    has_d2<double(const Vector&,const Vector&,const Vector&), const boost::type_erasure::_self> ,
    has_d2_dual<Vector(const Vector&,const Vector&), const boost::type_erasure::_self> ,
    has_hessian<LinearOperator(const Vector&), const boost::type_erasure::_self> ,
    has_solver<LinearSolver(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Functional. Plug your implementations in here.
   */
  using Functional = boost::type_erasure::any< boost::mpl::vector< ConceptBase , FunctionalConcept > >;

  /**
   * @brief Differentiable functional. Plug your implementations in here.
   */
  using C1Functional = boost::type_erasure::any< boost::mpl::vector< ConceptBase , C1FunctionalConcept > >;

  /**
   * @brief Twice differentiable functional. Plug your implementations in here.
   */
  using C2Functional = boost::type_erasure::any< boost::mpl::vector< ConceptBase , C2FunctionalConcept > >;
}

#endif // ALGORITHM_FUNCTIONAL_HH
