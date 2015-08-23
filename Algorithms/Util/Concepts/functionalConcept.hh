#ifndef ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH
#define ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "linearSolverConcept.hh"
#include "operatorConcept.hh"
#include "vectorConcept.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Concept for functionals.
   * @see ::Algorithm::Functional
   */
  using FunctionalConcept =
  boost::mpl::vector<
    ConceptBase ,
    boost::type_erasure::callable<double(const boost::type_erasure::any<VectorConcept>&),
                                  const boost::type_erasure::_self> ,
    has_domain<VectorSpace&()> ,
    has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
    has_domain_ptr<VectorSpace*(), const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept for differentiable functionals.
   * @see ::Algorithm::C1Functional
   */
  using C1FunctionalConcept =
  boost::mpl::vector<
    FunctionalConcept ,
    has_d1<double(const boost::type_erasure::any<VectorConcept>&, const boost::type_erasure::any<VectorConcept>&),
           const boost::type_erasure::_self> ,
    has_d1_dual<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                const boost::type_erasure::_self>
  >;

  /**
   * @brief Concept for twice differentiable functionals.
   * @see ::Algorithm::C2Functional
   */
  using C2FunctionalConcept =
  boost::mpl::vector<
    C1FunctionalConcept ,
    has_d2<double(const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
           const boost::type_erasure::_self> ,
    has_d2_dual<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
                const boost::type_erasure::_self> ,
    has_hessian<boost::type_erasure::any<LinearOperatorConcept>(const boost::type_erasure::any<VectorConcept>&),
                const boost::type_erasure::_self>
  >;
}

#endif // ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH
