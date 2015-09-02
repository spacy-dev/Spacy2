#ifndef ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
#define ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH

#include <boost/type_erasure/any.hpp>

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/Util/Mixins/eps.hh"
#include "Spacy/Util/Mixins/regularityTest.hh"
#include "Spacy/Util/Concepts/Newton/dampingStrategyConcept.hh"

namespace Spacy
{
  namespace Newton
  {
    /**
     * \brief Damping strategies for newton methods.
     * \anchor Newton_DampingStrategyAnchor
     * See \ref Newton_DampingStrategyConceptAnchor "DampingStrategyConcept".
     */
    using DampingStrategy = boost::type_erasure::any< ::Spacy::Concepts::Newton::DampingStrategyConcept >;

    namespace Damping
    {
      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.3].
       */
      class AffineCovariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineCovariant(const Operator& F);

        /// Compute damping factor.
        DampingFactor operator()(const LinearSolver& DFInv_, const Vector& x, const Vector& dx) const;

      private:
        const Operator& F_;

        mutable DampingFactor oldNu = -1;
        mutable double normOldDx = -1, normOldDs = -1;
        mutable Vector oldDs;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.2].
       */
      class AffineContravariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineContravariant(const Operator& F);

        /// Compute damping factor.
        DampingFactor operator()(const LinearSolver&, const Vector& x, const Vector& dx) const;

      private:
        const Operator& F_;
        mutable double muPrime = -1.;
        mutable double norm_F_x_old = -1;
      };

      /**
       * @ingroup NewtonGroup
       * @brief No damping, yields local newton method.
       */
      class None
      {
      public:
        /// Constructor.
        None(const Operator&);

        /**
         * @brief Compute damping factor.
         * @return 1
         */
        DampingFactor operator()(const LinearSolver&, const Vector&, const Vector&) const;
      };
    }
  }
}

#endif // ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
