#pragma once

#include <vector>

#include <Spacy/Util/Mixins/accuracy.hh>
#include <Spacy/Util/Mixins/Eps.hh>
#include <Spacy/Util/Mixins/maxSteps.hh>
#include <Spacy/Util/Mixins/verbosity.hh>


// added for extra step termination
#include <Spacy/vector.hh>
#include <string>
#include <iostream>
#include <limits>
#include <Spacy/Spaces/productSpace.hh>

namespace Spacy
{
  /** @addtogroup CGGroup  @{ */
  namespace CG
  {
    namespace Termination
    {
      /**
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
        /// termination decision
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

        /// re-initializes the termination criterion for a new CG run
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
        /// returns the estimated sqaured absolute energy error
        double squaredRelativeError() const noexcept;

        unsigned lookAhead_ = 5;
        std::vector<double> scaledGamma2 = std::vector<double>{};
        double energyNorm2 = 0;
        double stepLength2 = 0.;
      };

      /**
       * @brief Extra Termination criterion for conjugate gradient methods in the context of a composite step method with cg+constraint preconditioner
       * as solver.
       *
       * Implements special criteria for the termination of simplified normal step and normal step computation. Needs parameters
       * from the outer composite step algorithm, as termination decision is based on the current status of nonlinearity and convergence.
       */

      class StepTermination
      {
      public:

        /**
           * \brief constructor for normal step criterion
           *
           * \param dn0 offset from transformation of the system to a rhs with zero as dual variable
           * \param Gamma measure for nonlinearity of constraint (2*getDesiredContraction()/omegaC)
           * \param rho_elbow elbow space for tangential step
           * \param eta_tau loss in tangential damping due to inaccuracy in normal step (smaller than one)
           */
        StepTermination(const Vector &dn0, const Real &Gamma, const Real &rho_elbow, const Real& eta_tau)
          : dn0_(dn0), Gamma_(Gamma), rho_elbow_(rho_elbow), eta_tau_(eta_tau)
        {  type_ = "Normal"; }

        /**
           * \brief constructor for simplified normal step criterion
           *
           * \param dn0 offset from transformation of the system to a rhs with zero as dual variable
           * \param boundNormds upper bound for norm of simp. normal step to get the fraction \f$ \frac{\|ds\|}{\|dx\|} \f$ small
           * enough to not cause rejection of steps
           */
        StepTermination(const Vector &dn0, const Real &boundNormds) : dn0_(dn0), boundNormds_(boundNormds)
        { type_ = "SimplifiedNormal"; }


        /**
           * \brief default constructor
           */
        StepTermination()
        {
          type_ = "none";
        }


        /**
           * \brief termination decision
           *
           * \param cg_iterate current cg iterate
           * \param SquaredErrorNormEstimate squared norm of the error estimate of the cg error estimator
           * \param SquaredSolutionNormEstimate squared norm of the solution estimate of the cg error estimator
           */
        bool terminate(const Vector& cg_iterate, Real SquaredErrorNormEstimate, Real SquaredSolutionNormEstimate) const
        {

          if(type_ == "SimplifiedNormal")
          {
            return norm(cg_iterate + dn0_) < boundNormds_;
          }

          else if(type_ == "Normal")
          {

            bool terminate = false;
            auto norm2_offset = norm(dn0_)*norm(dn0_);
            auto norm2_tilde_dn_ex = SquaredSolutionNormEstimate;
            auto norm2_dn_err = SquaredErrorNormEstimate;
            auto GammaSquare = Gamma_ * Gamma_;

            // get primal element of x=(dn,p)
            auto w = cg_iterate;
            auto& w_ = cast_ref<ProductSpace::Vector>(w);
            w_.component(1) *= 0;
            auto norm2_dn = norm(dn0_ + w) * norm(dn0_ + w); // dn = dn0 + \tilde{dn} (this is NOT in the kernel of C!!)

            // assert that stopping criterion has a proper estimate
            if(norm2_tilde_dn_ex < std::numeric_limits<double>::max() && norm2_dn_err < std::numeric_limits<double>::max())
            {
              if (norm2_offset <= norm2_tilde_dn_ex)// in case something is wrong as 0 < |sol|^2 = |offset|^2 - |solest|^2
                std::cout << "OFFSET NORM <= Solution estimate!!! " << norm2_offset << " <= "
                          << norm2_tilde_dn_ex << std::endl;

              // norm of dn is small, i.e in trust region rho_elbow*Gamma
              if ((rho_elbow_ * GammaSquare >= norm2_dn) && (norm2_offset > norm2_tilde_dn_ex)) //assert that we dont take roots of negative numbers
              {
                // this estimates the fraction of loss in tangential damping due to a longer normal step
                auto taubound = min(1., sqrt(GammaSquare - norm2_dn) /
                                    sqrt(GammaSquare - (norm2_offset - norm2_tilde_dn_ex)))
                    - (sqrt(norm2_dn_err) /
                       sqrt(GammaSquare - (norm2_offset - norm2_tilde_dn_ex)));

                if (eta_tau_ + 1e-15 < taubound)
                {
                  std::cout << "terminating CG because of dn-stopping criterion: Case 1 (small norm)"<<std::endl;
                  std::cout << "   current error estimate " << sqrt(norm2_dn_err / norm2_tilde_dn_ex)
                            << std::endl;
                  terminate = true;
                }
              }
            }
            else
            {
              auto taubound = sqrt(GammaSquare - norm2_dn) - sqrt(norm2_dn);
              if (eta_tau_ + 1e-15 < taubound)
              {
                std::cout << "terminating CG because of dn-stopping criterion: Case 2 (no solution estimate)"<<std::endl;
                std::cout << "   current error estimate " << sqrt(norm2_dn_err / norm2_tilde_dn_ex)
                          << std::endl;
                terminate = true;
              }
            }
            return terminate;
          }

          else if(type_ == "none")
          {
            return false;
          }

          else
          {
            std::cout<< "No fitting type found in Termination Criterion! "<<std::endl;
            return false;
          }

        }

      protected:
        std::string type_ = "none";

        // For Normal Step Termination
        // The offset performed before calling the solver
        Vector dn0_;

        //2*Theta_des/omega_c
        Real Gamma_ = 0;

        //elbow space
        Real rho_elbow_ = 0;

        //threshold for normal step
        Real eta_tau_=0;

        // FOr Simplified Normal Step Termination
        //Contraction threshold for simpnorm step
        Real boundNormds_ = 0;
      };


      /**
       * @brief Termination criterion for conjugate gradient methods based on an estimate of the relative energy error.
       *
       * This error estimator provides an adaptive choice of the lookahead parameter.
       * In the context of the composite step method, it can use the extra termination criteria given by the StepTermination class.
       */
      class AdaptiveRelativeEnergyError :
          public Mixin::AbsoluteAccuracy,
          public Mixin::RelativeAccuracy,
          public Mixin::MinimalAccuracy,
          public Mixin::Eps,
          public Mixin::MaxSteps,
          public Mixin::Verbosity
      {
      public:
        /**
           * \brief Constructor for the error estimator in the context of composite step methods
           *
           * \param st StepTermination with an extra termination criterion
           */
        AdaptiveRelativeEnergyError(StepTermination st);

        /**
           * \brief Default constructor
           *
           */
        AdaptiveRelativeEnergyError();



        /**
           * \brief the termination decision function called by the cg
           * This is the decision from the StepTermination combined with the one of the error estimator errorEstimationTerminate()
           *
           */
        bool operator()() const;

        /**
           * \brief termination decision due to relative accuracy (independent of a composite step context)
           *
           */
        bool errorEstimationTerminate() const;
        /**
           * @brief supplies algorithmic quantities to the termination criterion
           * @param alpha scaling for the conjugate search direction \f$q\f$
           * @param qAq squared energy norm of the conjugate search direction \f$q\f$ (here: unused)
           * @param qPq squared \f$P\f$-norm, i. e. the norm induced by the preconditioner, of the conjugate search direction \f$q\f$ (here: unused)
           * @param rPINVr squared \f$P^{-1}\f$-norm of the residual
           * @param cg_iterate_ current cg_iterate
           */
        void update(double alpha, double qAq, double qPq, double rPINVr, const Vector& cg_iterate_);

        /**
           * @brief supplies algorithmic quantities to the termination criterion
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

        /// re-initializes the termination criterion for a new CG run
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
        double getSquaredErrorEstimator() const noexcept;
        double getSquaredSolutionEstimator() const noexcept;
        double squaredRelativeError() const noexcept;

        /**
           * \brief Standart Deviation of the lookahead as random variable
           * \return Standart Deviation
           */
        double getStdDev() const noexcept;

        Vector cg_iterate;
        mutable unsigned lookAhead_ = 1;
        std::vector<double> scaledGamma2 = std::vector<double>{};
        double energyNorm2 = 0;
        double stepLength2 = 0.;
        mutable double tau;
        const unsigned lookahead_min = 1;
        const double desiredtau = 0.5;

        StepTermination st_ = StepTermination();
      };
    }
  }
}
