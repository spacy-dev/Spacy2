#ifndef SPACY_CONCEPTS_DYNAMIC_OPERATOR_CONCEPT_HH
#define SPACY_CONCEPTS_DYNAMIC_OPERATOR_CONCEPT_HH

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "operatorConcept.hh"
#include "linearSolverConcept.hh"
#include "vectorConcept.hh"

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Concepts
  {
    /**
     * \ingroup ConceptGroup
     * \anchor DynamicCallableOperatorConceptAnchor
     * \brief Concept defining the simplest possible time-dependent operator, only providing Vector operator()(double t, const Vector&).
     *
     * The minimal signature of a time-dependent callable operator is:
     * \code
     * // My callable operator A: [0,T[ \times X -> Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(t,x).
     *   ::Spacy::Vector operator()(double t, const ::Spacy::Vector& x) const;
     * };
     * \endcode
     *
     * See \ref DynamicCallableOperatorAnchor "::Spacy::DynamicCallableOperator".
     */
    using DynamicCallableOperatorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(double t, const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor DynamicOperatorConceptAnchor
     * \brief Concept defining general time-dependent operators.
     *
     * The minimal signature of a time-dependent operator is:
     * \code
     * // My operator A: [0,T[ \times X -> Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(t,x).
     *   ::Spacy::Vector operator()(double t, const ::Spacy::Vector& x) const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access mass matrix.
     *   LinearOperator M() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref DynmaicOperatorAnchor "::Spacy::DynamicOperator".
     */
    using DynamicOperatorConcept =
    boost::mpl::vector<
      DynamicCallableOperatorConcept ,
      has_M<boost::type_erasure::any< LinearOperatorConcept >(), const boost::type_erasure::_self> ,
      has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
      has_range<const VectorSpace&(), const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor DynamicLinearOperatorConceptAnchor
     * \brief Concept defining time-dependent linear operators.
     *
     * The minimal signature of a time-dependent linear operator is:
     * \code
     * // My linear operator A: [0,T[ \times X -> Y.
     * class MyLinearOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyLinearOperator(const MyLinearOperator&);
     *
     *   // Move constructor.
     *   MyLinearOperator(MyLinearOperator&&);
     *
     *   // Compute A(t,x).
     *   ::Spacy::Vector operator()(double t, const ::Spacy::Vector& x) const;
     *
     *   // Access solver for the computation of A(t)^-1.
     *   ::Spacy::LinearSolver solver() const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref DynamicLinearOperatorAnchor "::Spacy::DynamicLinearOperator".
     */
    using DynamicLinearOperatorConcept =
    boost::mpl::vector<
      DynamicOperatorConcept ,
      has_solver<const boost::type_erasure::any<LinearSolverConcept>&(),
                 const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor DynamicC1OperatorConceptAnchor
     * \brief Concept defining time-dependent operators that are differentiable with respect to the spatial variable.
     *
     * The minimal signature for a time-dependent differentiable operator is:
     * \code
     * // My differentiable operator A: [0,T[ \times X -> Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(t,x).
     *   ::Spacy::Vector operator()(double t, const ::Spacy::Vector& x) const;
     *
     *   // Compute A_x(t,x)dx.
     *   ::Spacy::Vector d1(double t, const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;
     *
     *   // Get linearization representing A_x(t,x).
     *  ::Spacy::LinearOperator linearization(double t, const ::Spacy::Vector& x) const;
     *
     *   // Access mass matrix.
     *   LinearOperator M() const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref DynamicC1OperatorAnchor "::Spacy::DynamicC1Operator".
     */
    using DynamicC1OperatorConcept =
    boost::mpl::vector<
      DynamicOperatorConcept ,
      has_d1<boost::type_erasure::any<VectorConcept>(double t, const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
             const boost::type_erasure::_self> ,
      has_linearization<boost::type_erasure::any<LinearOperatorConcept>(double t, const boost::type_erasure::any<VectorConcept>&),
                        const boost::type_erasure::_self>
    >;
  }
}

#endif // SPACY_CONCEPTS_DYNAMIC_OPERATOR_CONCEPT_HH
