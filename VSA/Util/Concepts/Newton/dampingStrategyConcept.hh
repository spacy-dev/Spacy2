#ifndef ALGORITHMS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH_
#define ALGORITHMS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH_

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>

#include "VSA/Util/Concepts/conceptBase.hh"
#include "VSA/Util/Concepts/linearSolverConcept.hh"
#include "VSA/Util/Concepts/vectorConcept.hh"

namespace VSA
{
  /// \cond
  class DampingFactor;
  /// \endcond

  namespace Concepts
  {
    namespace Newton
    {
      /**
       * @ingroup NewtonConceptGroup
       * @anchor Newton_DampingStrategyConceptAnchor
       * @brief Concept for damping strategies for %Newton's method.
       *
       * The minimal signature of a damping strategy is:
       * @code
       * // My damping strategy.
       * class MyDampingStrategy
       * {
       * public:
       *   // Copy constructor.
       *   MyDampingStrategy(const MyDampingStrategy&);
       *
       *   // Move constructor.
       *   MyDampingStrategy(MyDampingStrategy&&);
       *
       *   // Compute damping factor.
       *   DampingFactor operator()(const LinearSolver& DF_inv, const Vector& x, const Vector& dx) const;
       * };
       * @endcode
       *
       * @see @ref Newton_DampingStrategyAnchor "Newton::DampingStrategy", @ref LinearSolverAnchor "LinearSolver",
       * @ref VectorAnchor "Vector", DampingFactor
       */
      using DampingStrategyConcept =
      boost::mpl::vector<
        ::VSA::Concepts::ConceptBase ,
        boost::type_erasure::callable< ::VSA::DampingFactor(const boost::type_erasure::any< ::VSA::Concepts::LinearSolverConcept >&,
                                                    const boost::type_erasure::any< ::VSA::Concepts::VectorConcept >&,
                                                    const boost::type_erasure::any< ::VSA::Concepts::VectorConcept >&),
                                      const boost::type_erasure::_self>
      >;
    }
  }
}

#endif // ALGORITHMS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH
