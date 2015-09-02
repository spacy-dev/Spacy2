#ifndef ALGORITHM_CONJUGATE_GRADIENTS_TERMINATION_CRITERION
#define ALGORITHM_CONJUGATE_GRADIENTS_TERMINATION_CRITERION

#include <vector>

#include <boost/type_erasure/any.hpp>

#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Concepts/CG/terminationCriterionConcept.hh"

namespace Spacy
{
  namespace CG
  {
    /**
     * @ingroup CGGroup
     * @brief Termination criteria for conjugate gradient methods.
     * @anchor CG_TerminationCriterionAnchor
     * See @ref CG_TerminationCriterionConceptAnchor "TerminationCriterionConcept".
     */
    using TerminationCriterion = boost::type_erasure::any< Concepts::CG::TerminationCriterionConcept >;

    namespace Termination
    {
      /**
       * @ingroup CGGroup
       * @brief Termination criterion for conjugate gradient methods based on an estimate of the relative energy error.
       *
       * Relative energy error termination criterion according to @cite Strakos2005 (see also @cite Hestenes1952, @cite Arioli2004).
       * Requires that CG starts at \f$ x = 0 \f$. More general starting values might be used, but must be chosen such that
       * the estimate for the energy norm of the solution stays positive (see the above mentioned paper for details).
       */
      class StrakosTichyEnergyError :
          public Mixin::AbsoluteAccuracy,
          public Mixin::RelativeAccuracy,
          public Mixin::MinimalAccuracy,
          public Mixin::Eps,
          public Mixin::MaxSteps,
          public Mixin::Verbosity
      {
      public:
        /**
         * \brief termination decision
         * \return true if the iteration has reached the required accuracy
         */
        bool operator()() const;

        /**
         * @brief addStepQuantities supplies algorithmic quantities to the termination criterion
         * @param alpha scaling for the conjugate search direction \f$q\f$
         * @param qAq squared energy norm of the conjugate search direction \f$q\f$ (here: unused)
         * @param qPq squared \f$P\f$-norm, i. e. the norm induced by the preconditioner, of the conjugate search direction \f$q\f$ (here: unused)
         * @param rPINVr squared \f$P^{-1}\f$-norm of the residual
         */
        void update(double alpha, double qAq, double qPq, double rPINVr);

        /**
         * \brief check if the energy norm of the current step \f$\|q\|_A=\sqrt(qAq)\f$ is smaller than the maximal attainable accuracy multiplied with the energy norm of the iterate \f$\varepsilon_{max}\|x\|_A\f$.
         * \return true if \f$\|q\|<\varepsilon_{max}\|x\|_A\f$, else false
         */
        bool vanishingStep() const noexcept;

        /**
         * \brief re-initializes the termination criterion for a new CG run
         */
        void clear() noexcept;

        /**
         * \brief set requested lookahead value
         *
         * \param lookAhead the requested lookahead (nonnegative)
         *
         * The default value is 5.
         */
        void setLookAhead(unsigned lookAhead) noexcept;

        /**
         * \brief relaxed termination decision
         * \return true if the iteration has reached some minimal required accuracy, possibly bigger than the desired accuracy. This method is required in the hybrid conjugate gradient method only.
         */
        bool minimalDecreaseAchieved() const noexcept;

      private:
        /**
         * \brief returns the estimated sqaured absolute energy error
         */
        double squaredRelativeError() const noexcept;

        unsigned lookAhead_ = 5;
        std::vector<double> scaledGamma2 = std::vector<double>{};
        double energyNorm2 = 0;
        double stepLength2 = 0.;
      };


      //  /**
      //   * Relative error termination criterion based on the norm induced by the preconditioner,
      //   * according to Strakos, Tichy: Error estimation in preconditioned conjugate gradients.
      //   * Requires that CG starts at \f$ x = 0 \f$. More general starting values might be used, but must be chosen such that
      //   * the estimate for the energy norm of the solution stays positive (see the above mentioned paper for details).
      //   */
      //  template <class R>
      //  class StrakosTichyPTerminationCriterion: public CGTerminationCriterion<R> {
      //  public:
      //    typedef R double;

      //    /**
      //     * \brief constructor
      //     *
      //     * The pcg iteration is terminated as soon as either the estimated error satisfies \f$ [\epsilon] \le \mathrm{tol} \f$ or
      //     * the number of iterations exceeds the limit maxit. Note that the estimate of the relative error requires a look ahead
      //     * parameter L. Thus, if \f$\mathrm{maxit}\ge\mathrm{L}\f$, then the error is first estimated in the (L+1)-th iteration.
      //     *
      //     * \param tol the relative error tolerance for termination
      //     * \param maxit the maximum number of iterations
      //     * \param eps maximal attainable accuracy
      //     */
      //    StrakosTichyPTerminationCriterion(double tol_, int maxit_, double eps_ = 1e-12)
      //      : tol(tol_), maxit(maxit_), eps(eps_)
      //    {}

      //    /**
      //     * \brief constructor
      //     *
      //     * The pcg iteration is terminated as soon as either the estimated error satisfies \f$ [\epsilon] \le \mathrm{tol} \f$ or
      //     * the number of iterations exceeds the limit maxit. Note that the estimate of the relative error requires a look ahead
      //     * parameter L. Thus, if \f$\mathrm{maxit}\ge\mathrm{L}\f$, then the error is first estimated in the (L+1)-th iteration.
      //     *
      //     * \param tol the relative error tolerance for termination
      //     * \param minTol relative error tolerance to admit truncation in the hybrid cg implementation
      //     * \param maxit the maximum number of iterations
      //     * \param eps maximal attainable accuracy
      //     */
      //    StrakosTichyPTerminationCriterion(double tol_, double minTol_, int maxit_, double eps_ = 1e-12)
      //      : tol(tol_), maxit(maxit_), eps(eps_), minTol(minTol_)
      //    {}

      //    /**
      //     * \brief re-initializes the termination criterion for a new CG run
      //     */
      //    virtual void clear()
      //    {
      //      scaledGamma2.clear();
      //      energyNorm = 0;
      //    }

      //    /**
      //     * \brief set requested relative tolerance
      //     *
      //     * \param tol the requested tolerance (nonnegative)
      //     */
      //    virtual void tolerance(double tol_) { tol = tol_; }

      //    /**
      //     * \brief set requested lookahead value
      //     *
      //     * \param lah the requested lookahead (nonnegative)
      //     *
      //     * The default value is 50.
      //     */
      //    virtual void lookahead(int d_) { d = d_; }

      //    /**
      //     * @brief addStepQuantities supplies algorithmic quantities to the termination criterion
      //     * @param stepLength scaling for the conjugate search direction \f$q\f$
      //     * @param qAq squared energy norm of the conjugate search direction \f$q\f$ (here: unused)
      //     * @param qPq squared \f$P\f$-norm, i. e. the norm induced by the preconditioner, of the conjugate search direction \f$q\f$ (here: unused)
      //     * @param rPINVr squared \f$P^{-1}\f$-norm of the residual
      //     */
      //    virtual void addStepQuantities(double alpha, double qAq, double qPq, double rPINVr)
      //    {
      //      scaledGamma2.push_back( alpha * rPINVr );
      //      energyNorm += alpha * rPINVr;
      //      steps2.push_back(qPq/qAq);
      //      stepLength = sqrt(std::fabs(qAq));
      //    }

      //    /**
      //     * \brief termination decision
      //     * \return true if the iteration has reached the required accuracy
      //     */
      //    virtual operator bool()
      //    {
      //      return scaledGamma2.size() > maxit || ( scaledGamma2.size() > 2*d && relativeError() < std::max(eps,tol*tol) );
      //    }

      //    /**
      //     * \brief returns the estimated absolute energy error
      //     */
      //    double relativeError()
      //    {
      //      if( scaledGamma2.size() < 2*d ) return std::numeric_limits<double>::max();

      //      size_t j = scaledGamma2.size() - 2*d;
      //      double tau = 0;
      //      for(size_t i = j; i < j + d; ++i)
      //        tau += steps2[i] * ( scaledGamma2[i] + 2 * std::accumulate(scaledGamma2.begin()+i+1, scaledGamma2.end(),0)  );
      //      return tau;
      //    }

      //    bool minimalDecreaseAchieved() { return relativeError() < minTol; }

      //    bool vanishingStep() const
      //    {
      //      return stepLength < eps * energyNorm;
      //    }


      //  private:
      //    double tol;
      //    int maxit;
      //    // squared gammas
      //    std::vector<double> scaledGamma2, steps2;
      //    double energyNorm = 0, stepLength = 0;
      //    int d = 50;
      //    double eps = 1e-12, minTol = 1e-2;
      //  };
    }
  }
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_TERMINATION_CRITERION
