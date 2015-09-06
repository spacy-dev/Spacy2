#ifndef AFFINECOVARIANTTERMINATIONCRITERION_HH
#define AFFINECOVARIANTTERMINATIONCRITERION_HH

#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Concepts/Newton/terminationCriterionConcept.hh"

namespace Spacy
{
  /// @cond
  class DampingFactor;
  /// @endcond

  namespace Newton
  {    
    /**
     * \brief Termination criteria for newton methods.
     * \anchor Newton_TerminationCriterionAnchor
     * See \ref Newton_TerminationCriterionConceptAnchor "TerminationCriterionConcept".
     */
    using TerminationCriterion = boost::type_erasure::any< Concepts::Newton::TerminationCriterionConcept >;

    namespace Termination
    {
      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant relative error criterion.
       */
      class AffineCovariant :
          public Mixin::RelativeAccuracy,
          public Mixin::Verbosity,
          public Mixin::Eps
      {
      public:
        /**
         * @brief Constuctor.
         */
        AffineCovariant(const Operator&, double relativeAccuracy, bool verbose = false);

        /**
         * @brief Apply termination criterion.
         * @param nu damping factor
         * @param x iterate
         * @param dx correction
         * @return true if \f$\nu=1\f$ and \f$ \|dx\|<rel_acc\|x\| \f$ or \f$\|x\|=\|dx\|=0\f$, else false
         */
        bool operator()(DampingFactor nu, const Vector& x, const Vector& dx) const;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant relative error criterion.
       */
      class AffineContravariant :
          public Mixin::RelativeAccuracy,
          public Mixin::Verbosity,
          public Mixin::Eps
      {
      public:
        /**
         * @brief Constructor.
         */
        AffineContravariant(const Operator& F, double relativeAccuracy, bool verbose = false);

        /**
         * @brief Apply termination criterion.
         * @param nu damping factor
         * @param x iterate
         * @return true if \f$\nu=1\f$ and \f$ \|F(x)\|<rel_acc\|F(x_0)\| \f$, else false
         */
        bool operator()(DampingFactor nu, const Vector& x, const Vector&) const;

      private:
        const Operator& F_;
        mutable Real initialResidual = Real{-1};
      };
    }
  }
}

#endif // AFFINECOVARIANTTERMINATIONCRITERION_HH
