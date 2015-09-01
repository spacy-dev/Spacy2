#ifndef ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH
#define ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH

#include "conceptBase.hh"
#include "singleConcepts.hh"
#include "linearSolverConcept.hh"
#include "vectorConcept.hh"

namespace VSA
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace Concepts
  {
    /**
     * \ingroup ConceptGroup
     * \anchor CallableOperatorConceptAnchor
     * \brief Concept defining the simplest possible operator, only providing Vector operator()(const Vector&).
     *
     * The minimal signature of a callable operator is:
     * \code
     * // My callable operator A: X->Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(x).
     *   ::VSA::Vector operator()(const ::VSA::Vector& x) const;
     * };
     * \endcode
     *
     * See \ref CallableOperatorAnchor "::VSA::CallableOperator".
     */
    using CallableOperatorConcept =
    boost::mpl::vector<
      ConceptBase ,
      boost::type_erasure::callable<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&),
                                    const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor OperatorConceptAnchor
     * \brief Concept defining general operators.
     *
     * The minimal signature of an operator is:
     * \code
     * // My operator A: X->Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(x).
     *   ::VSA::Vector operator()(const ::VSA::Vector& x) const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref OperatorAnchor "::VSA::Operator".
     */
    using OperatorConcept =
    boost::mpl::vector<
      CallableOperatorConcept ,
      has_domain<const VectorSpace&(), const boost::type_erasure::_self> ,
      has_range<const VectorSpace&(), const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor LinearOperatorConceptAnchor
     * \brief Concept defining linear operators.
     *
     * The minimal signature of a linear operator is:
     * \code
     * // My linear operator A: X->Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(x).
     *   ::VSA::Vector operator()(const ::VSA::Vector& x) const;
     *
     *   // Access solver for the computation of A^-1.
     *   ::VSA::LinearSolver solver() const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref LinearOperatorAnchor "::VSA::LinearOperator".
     */
    using LinearOperatorConcept =
    boost::mpl::vector<
      OperatorConcept ,
      has_solver<const boost::type_erasure::any<LinearSolverConcept>&(),
                 const boost::type_erasure::_self>
    >;

    /**
     * \ingroup ConceptGroup
     * \anchor C1OperatorConceptAnchor
     * \brief Concept defining differentiable operators.
     *
     * The minimal signature for a differentiable operator is:
     * \code
     * // My differentiable operator A: X->Y.
     * class MyOperator
     * {
     * public:
     *   // Copy constructor.
     *   MyOperator(const MyOperator&);
     *
     *   // Move constructor.
     *   MyOperator(MyOperator&&);
     *
     *   // Compute A(x).
     *   ::VSA::Vector operator()(const ::VSA::Vector& x) const;
     *
     *   // Compute A'(x)dx.
     *   ::VSA::Vector d1(const ::VSA::Vector& x, const ::VSA::Vector& dx) const;
     *
     *   // Get linearization representing A'(x).
     *  ::VSA::LinearOperator linearization(const ::VSA::Vector& x) const;
     *
     *   // Access domain space.
     *   const VectorSpace& domain() const;
     *
     *   // Access range space.
     *   const VectorSpace& range() const;
     * };
     * \endcode
     *
     * See \ref C1OperatorAnchor "::VSA::C1Operator".
     */
    using C1OperatorConcept =
    boost::mpl::vector<
      OperatorConcept ,
      has_d1<boost::type_erasure::any<VectorConcept>(const boost::type_erasure::any<VectorConcept>&,const boost::type_erasure::any<VectorConcept>&),
             const boost::type_erasure::_self> ,
      has_linearization<boost::type_erasure::any<LinearOperatorConcept>(const boost::type_erasure::any<VectorConcept>&),
                        const boost::type_erasure::_self>
    >;
  }
}

#endif // ALGORITHM_CONCEPTS_OPERATOR_CONCEPT_HH
