#ifndef SPACYS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH_
#define SPACYS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH_

#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/callable.hpp>

#include "Spacy/Util/Concepts/conceptBase.hh"
#include "Spacy/Util/Concepts/vectorConcept.hh"

namespace Spacy
{
  /// @cond
  class DampingFactor;
  class Vector;
  /// @endcond

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
        ::Spacy::Concepts::ConceptBase ,
        boost::type_erasure::callable< ::Spacy::DampingFactor(const std::function<Vector(const Vector&)>&,
                                                    const Vector&,
                                                    const Vector&),
                                      const boost::type_erasure::_self>
//      boost::type_erasure::callable< ::Spacy::DampingFactor(const boost::type_erasure::any< ::Spacy::Concepts::LinearSolverConcept >&,
//                                                  const boost::type_erasure::any< ::Spacy::Concepts::VectorConcept >&,
//                                                  const boost::type_erasure::any< ::Spacy::Concepts::VectorConcept >&),
//                                    const boost::type_erasure::_self>
      >;
    }
  }
}

#endif // SPACYS_CONCEPTS_NEWTON_DAMPING_STRATEGY_CONCEPT_HH
