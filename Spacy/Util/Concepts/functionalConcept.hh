#ifndef ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH
#define ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "linearSolverConcept.hh"
#include "operatorConcept.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Concepts
  {
    /**
     * @ingroup ConceptGroup
     * @anchor FunctionalConceptAnchor
     * @brief Concept for functionals.
     *
     * The minimal signature of a functional is:
     * @code
     * // A functional f: X->R.
     * class MyFunctional
     * {
     * public:
     *   // Compute f(x).
     *   double operator()(const ::Spacy::Vector&) const;
     *
     *   // Access underlying domain.
     *   const VectorSpace& domain() const;
     * };
     * @endcode
     *
     * See @ref FunctionalAnchor "::Spacy::Functional".
     */
    using FunctionalConcept =
    boost::mpl::vector<
      ConceptBase,
      boost::type_erasure::callable<double(const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>,
      has_domain<const VectorSpace&(), const boost::type_erasure::_self>
    >;

    /**
     * @ingroup ConceptGroup
     * @anchor C1FunctionalConceptAnchor
     * @brief Concept for differentiable functionals.
     *
     * The minimal signature of a differentiable functional is
     * @code
     * // A functional f: X->R.
     * class MyFunctional
     * {
     * public:
     *   // Copy constructor, possibly default-generated.
     *   MyFunctional(const MyFunctional&);
     *
     *   // Move constructor, possibly default-generated.
     *   MyFunctional(MyFunctional&&);
     *
     *   // Compute f(x).
     *   double operator()(const ::Spacy::Vector&) const;
     *
     *
     *
     *   // Access underlying domain.
     *   const VectorSpace& domain() const;
     * };
     * @endcode
     *
     * See @ref C1FunctionalAnchor "::Spacy::C1Functional".
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
     * @ingroup ConceptGroup
     * @anchor C2FunctionalConceptAnchor
     * @brief Concept for twice differentiable functionals.
     * See @ref C2FunctionalAnchor "::Spacy::C2Functional".
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
}

#endif // ALGORITHM_CONCEPTS_FUNCTIONAL_CONCEPT_HH
