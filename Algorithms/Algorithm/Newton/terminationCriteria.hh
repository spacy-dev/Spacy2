#ifndef AFFINECOVARIANTTERMINATIONCRITERION_HH
#define AFFINECOVARIANTTERMINATIONCRITERION_HH

#include "Algorithms/operator.hh"
#include "Algorithms/vector.hh"
#include "Algorithms/Util/mixins.hh"
#include "Algorithms/Util/Concepts/Newton/terminationCriterionConcept.hh"

namespace Algorithm
{
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
        bool operator()(double nu, const Vector& x, const Vector& dx) const;
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
        bool operator()(double nu, const Vector& x, const Vector&) const;

      private:
        const Operator& F_;
        mutable double initialResidual = -1;
      };
    }
  }
}

#endif // AFFINECOVARIANTTERMINATIONCRITERION_HH
