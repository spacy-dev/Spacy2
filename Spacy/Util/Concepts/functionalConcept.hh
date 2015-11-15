#ifndef SPACY_CONCEPTS_FUNCTIONAL_CONCEPT_HH
#define SPACY_CONCEPTS_FUNCTIONAL_CONCEPT_HH

#include <boost/type_erasure/callable.hpp>
#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "operatorConcept.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  /// \cond
  class Real;
  class Vector;
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
     *   double operator()(const ::Spacy::Vector& x) const;
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
    boost::type_erasure::callable<Real(const Vector&),
                                  const boost::type_erasure::_self>,
//    boost::type_erasure::callable<double(const boost::type_erasure::any<VectorConcept>&),
//                                  const boost::type_erasure::_self>,
      has_domain<const VectorSpace&(), const boost::type_erasure::_self>
    >;

    /**
     * @ingroup ConceptGroup
     * @anchor C1FunctionalConceptAnchor
     * @brief Concept for differentiable functionals.
     *
     * The minimal signature of a differentiable functional is
     * @code
     * // A differentiable functional f: X->R.
     * class MyC1Functional
     * {
     * public:
     *   // Copy constructor, possibly default-generated.
     *   MyC1Functional(const MyC1Functional&);
     *
     *   // Move constructor, possibly default-generated.
     *   MyC1Functional(MyC1Functional&&);
     *
     *   // Compute f(x).
     *   double operator()(const ::Spacy::Vector& x) const;
     *
     *   // Compute f'(x) as element of X*.
     *   ::Spacy::Vector d1(const ::Spacy::Vector& x) const;
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
    has_d1_dual<Vector(const Vector&),
                const boost::type_erasure::_self>
//    has_d1_dual<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
//                const boost::type_erasure::_self>
    >;

    /**
     * @ingroup ConceptGroup
     * @anchor C2FunctionalConceptAnchor
     * @brief Concept for twice differentiable functionals.
     *
     * The minimal signature of a twice differentiable functional is
     * @code
     * // A twice differentiable functional f: X->R.
     * class MyC2Functional
     * {
     * public:
     *   // Copy constructor, possibly default-generated.
     *   MyC2Functional(const MyC2Functional&);
     *
     *   // Move constructor, possibly default-generated.
     *   MyC2Functional(MyC2Functional&&);
     *
     *   // Compute f(x).
     *   double operator()(const ::Spacy::Vector& x) const;
     *
     *   // Compute f'(x) as element of X*.
     *   ::Spacy::Vector d1(const ::Spacy::Vector& x) const;
     *
     *   // Compute f''(x)dx as element of X*.
     *   ::Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;
     *
     *   // Access f''(x) as mapping f''(x): X->X*.
     *   LinearOperator hessian(const ::Spacy::Vector& x);
     *
     *   // Access underlying domain.
     *   const VectorSpace& domain() const;
     * };
     * @endcode
     *
     * See @ref C2FunctionalAnchor "::Spacy::C2Functional".
     */
    using C2FunctionalConcept =
    boost::mpl::vector<
      C1FunctionalConcept ,
    has_d2_dual<Vector(const Vector&,const Vector&),
                const boost::type_erasure::_self> ,
    has_hessian<boost::type_erasure::any<LinearOperatorConcept>(const Vector&),
                const boost::type_erasure::_self>
//    has_d2_dual<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
//                const boost::type_erasure::_self> ,
//    has_hessian<boost::type_erasure::any<LinearOperatorConcept>(const boost::type_erasure::any<VectorConcept>&),
//                const boost::type_erasure::_self>
    >;
  }
}

#endif // SPACY_CONCEPTS_FUNCTIONAL_CONCEPT_HH
