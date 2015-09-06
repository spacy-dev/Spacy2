#ifndef SPACY_FUNCTIONAL_HH
#define SPACY_FUNCTIONAL_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/functionalConcept.hh"

#include "vectorSpace.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @anchor FunctionalAnchor
   * @brief Functional. Can store objects that satisfy the requirements of \ref FunctionalConceptAnchor "FunctionalConcept".
   */
  using Functional = boost::type_erasure::any<Concepts::FunctionalConcept>;

  /**
   * @ingroup SpacyGroup
   * @anchor C1FunctionalAnchor
   * @brief Differentiable functional. Can store objects that satisfy the requirements of \ref C1FunctionalConceptAnchor "C1FunctionalConcept".
   */
  using C1Functional = boost::type_erasure::any<Concepts::C1FunctionalConcept>;

  /**
   * @ingroup SpacyGroup
   * @anchor C2FunctionalAnchor
   * @brief Twice differentiable functional. Can store objects that satisfy the requirements of \ref C2FunctionalConceptAnchor "C2FunctionalConcept".
   */
  using C2Functional = boost::type_erasure::any<Concepts::C2FunctionalConcept>;


  /**
   * @ingroup SpacyGroup
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f'\f$ at \f$x\in X\f$ as dual element \f$ f'(x) \in X^* \f$.
   *
   * This function is provided for providing unified access to the k-th derivative via dk(f,x)(arg0)...(argn).
   * Equivalent to calling f.d1(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f'(x)\f$, i.e. f.d1(x).
   * @see @ref C2FunctionalAnchor "C2Functional"
   */
  boost::type_erasure::any< Concepts::VectorConcept > d1(const C2Functional& f, const boost::type_erasure::any< Concepts::VectorConcept >& x);

  /**
   * @ingroup SpacyGroup
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f''\f$ at \f$x\in X\f$ as linear opeator \f$ f''(x): X \to X^* \f$.
   *
   * This function is provided for providing unified access to the k-th derivative via dk(f,x)(arg0)...(argn).
   * Equivalent to calling f.hessian(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f''(x)\f$, i.e. f.hessian(x).
   * @see @ref C2FunctionalAnchor "C2Functional"
   */
  boost::type_erasure::any< Concepts::LinearOperatorConcept > d2(const C2Functional& f, const boost::type_erasure::any< Concepts::VectorConcept >& x);
}

#endif // SPACY_FUNCTIONAL_HH
