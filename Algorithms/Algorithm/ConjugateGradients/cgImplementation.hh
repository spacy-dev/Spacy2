#ifndef ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "operator.hh"
#include "functionSpaceElement.hh"
#include "Util/Exceptions/singularOperatorException.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/iterativeRefinements.hh"
#include "Util/Mixins/verbosity.hh"

#include "cgTerminationCriteria.hh"

namespace Algorithm
{
  enum class CGImplementationType { STANDARD, TRUNCATED, REGULARIZED, TRUNCATED_REGULARIZED };

  namespace CG_Detail
  {
    class NoRegularization :
        public Mixin::Eps,
        public Mixin::Verbosity
    {
    public:
      NoRegularization(double eps ,bool verbose) noexcept;

      void initializeRegularization() const noexcept;
      void regularize(double&, double) const noexcept;
      void updateRegularization(double,double) const noexcept;
      template <class X>
      void adjustRegularizedResidual(double, const X&, const X&) const noexcept {}
    };


    class Regularization :
        public Mixin::Eps,
        public Mixin::Verbosity
    {
    public:
      Regularization(double eps, bool verbose) noexcept;

      void initializeRegularization() const noexcept;

      void regularize(double& qAq, double qPq) const noexcept;

      void updateRegularization(double qAq, double qPq) const;

      template <class X>
      void adjustRegularizedResidual(double alpha, const X& Pq, X& r) const
      {
        r -= (alpha*theta)*Pq;
      }

    private:
      mutable double theta = 0;
      unsigned maxIncrease = 1000, minIncrease = 2;
    };


    template <CGImplementationType Impl>
    using ChooseRegularization = typename std::conditional< ( Impl==CGImplementationType::STANDARD || Impl==CGImplementationType::TRUNCATED ),
    NoRegularization , Regularization >::type;
  } // end namespace CG_Detail



  /**
   * \ingroup linalgsolution
   * \brief regularized preconditioned conjugate gradient method
   *
   * This implements a preconditioned CG iteration for an operator \f$ A: X\to x^* \f$, preconditioned by a
   * preconditioner \f$ B^{-1}: X^* \to X \f$. The termination is based on an estimate of the absolute energy error.
   *
   * The implementation follows Deuflhard/Weiser, Section 5.3.3.
   *
   */
  template<CGImplementationType Impl = CGImplementationType::STANDARD>
  class CGImpl : public CG_Detail::ChooseRegularization<Impl> , public Mixin::IterativeRefinements
  {
    enum class Result { Converged, Failed, EncounteredNonConvexity, TruncatedAtNonConvexity };
  public:
    /**
     * \brief Set up conjugate gradient solver.
     *
     * \param A_ linear operator
     * \param P_ preconditioner
     * \param verbose_ print information on the cg iteration with verbose_=true (default=false).
     * \param eps_ estimate for the maximal attainable accuracy (default=false).
     */
    CGImpl(const Operator& A, const Operator& P, bool verbose_ = true, double eps_ = 1e-12)
      : CG_Detail::ChooseRegularization<Impl>(eps_,verbose_),
        A_(A), P_(P), terminate(std::make_unique< RelativeEnergyError >())
    {
      initPre();
      terminate->setEps(eps_);
    }

    /**
     * @param x initial guess
     * @param b right hand side
     * @param tolerance tolerance of the termination criterion
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b, double tolerance) const
    {
      terminate->setRelativeAccuracy(tolerance);
      return solve(x,b);
    }

    /**
     * @param b right hand side
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& b) const
    {
      auto x = FunctionSpaceElement( A_.impl().getDomain().element() );
      return solve(x,b);
    }

    /**
     * @param x initial guess
     * @param b right hand side
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b) const
    {
      this->initializeRegularization();
      if( Impl == CGImplementationType::STANDARD || Impl == CGImplementationType::TRUNCATED )
        return cgLoop(x,b);
      else
      {
        FunctionSpaceElement y = x;
        while( result != Result::Converged && result != Result::TruncatedAtNonConvexity )
          y = cgLoop(x,b);
        return y;
      }
    }


    /// Set a new termination criterion which must inherit from CGTerminationCriterion.
    template <class Criterion>
    void setTerminationCriterion(const Criterion& newTerminate)
    {
      terminate.reset(std::make_unique<Criterion>(newTerminate));
    }

    /// Access to the termination criterion, i.e. for adjusting parameters.
    CGTerminationCriterion& getTerminationCriterion() noexcept
    {
      assert(terminate!=nullptr);
      return *terminate;
    }

    /// Tells us whether non-convex directions occurred.
    bool encounteredNonConvexity() const noexcept
    {
      return result==Result::EncounteredNonConvexity || result==Result::TruncatedAtNonConvexity;
    }

    /// Energy norm of the computed solution.
    auto getEnergyNormOfSolution() const noexcept
    {
      return sqrt(energyNorm2);
    }

  private:
    using CG_Detail::ChooseRegularization<Impl>::eps;
    using CG_Detail::ChooseRegularization<Impl>::verbose;

    /// CG Implementation.
    FunctionSpaceElement cgLoop (FunctionSpaceElement x, FunctionSpaceElement r) const
    {
      terminate->clear();
      result = Result::Failed;

      // initialization phase for conjugate gradients
      auto Ax = A_(x);
      r -= Ax;
      auto Qr = Q(r);

      auto q = Qr;
      auto Pq = r; // required only for regularized or hybrid conjugate gradient methods

      auto sigma = std::abs( r(Qr) ); // preconditioned residual norm squared

      // the conjugate gradient iteration
      for (unsigned step = 1; true; step++ )
      {
        if( verbose() ) std::cout << "Iteration: " << step << std::endl;
        auto Aq = A_(q);
        auto qAq = Aq(q);
        auto qPq = Pq(q);

        auto alpha = sigma/qAq;

        terminate->provideAlgorithmicQuantities(alpha,qAq,qPq,sigma);

        //  don't trust small numbers
        if( vanishingStep() ) break;
        if( terminateOnNonconvexity(qAq,qPq,x,q,step) ) break;

        x += alpha * q;
        energyNorm2 += alpha*qAq;

        // convergence test
        if (*terminate)
        {
          if( verbose() ) std::cout << pre << "Terminating in iteration " << step << ".\n";
          result = terminate->reachedMaximalNumberOfIterations() ? Result::Failed : Result::Converged;
          break;
        }

        r -= alpha*Aq;
        this->adjustRegularizedResidual(alpha,Pq,r);

        Qr = Q(r);

        // determine new search direction
        auto sigmaNew = std::abs( r(Qr) ); // sigma = <Qr,r>
        auto beta = sigmaNew/sigma;
        sigma = sigmaNew;

        q *= beta; q += Qr;  //  q = Qr + beta*q
        Pq *= beta; Pq += r; // Pq = r + beta*Pq
      }

      return x;
    }

    FunctionSpaceElement Q(const FunctionSpaceElement& r) const
    {
      auto Qr = P_(r);
      for(auto i=0u; i<iterativeRefinements(); ++i)
        Qr += P_(r-A_(Qr));
      return Qr;
    }

    /// Set string pre.
    void initPre()
    {
      if(Impl == CGImplementationType::STANDARD) pre = std::string("CG: ");
      if(Impl == CGImplementationType::TRUNCATED) pre = std::string("TCG: ");
      if(Impl == CGImplementationType::REGULARIZED) pre = std::string("RCG: ");
      if(Impl == CGImplementationType::TRUNCATED_REGULARIZED) pre = std::string("TRCG: ");
    }

    /// Check step length.
    bool vanishingStep() const
    {
      if( terminate->vanishingStep() )
      {
        if( verbose() ) std::cout << pre << "Terminating due to numerically almost vanishing step." << std::endl;
        result = Result::Converged;
        return true;
      }
      return false;
    }

    /**
     * \return true if iteration should terminate.
     */
    template <class Scalar>
    bool terminateOnNonconvexity(Scalar qAq, Scalar qPq, FunctionSpaceElement& x, const FunctionSpaceElement& q, unsigned step) const
    {
      if( qAq > 0 ) return false;

      if( Impl == CGImplementationType::STANDARD )
      {
        if( verbose() )
        {
          std::cout << pre << "Direction of negative curvature encountered in standard CG Implementation!" << std::endl;
          std::cout << pre << "Either something is wrong with your operator or you should use TCG, RCG or HCG. Terminating CG!" << std::endl;
        }

        throw SingularOperatorException("CG::terminateOnNonconvexity");
      }

      if( Impl == CGImplementationType::TRUNCATED || ( Impl == CGImplementationType::TRUNCATED_REGULARIZED && terminate->minimalDecreaseAchieved() ) )
      {
        // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is encountered in the first step something probably went wrong
        // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
        if( step == 1 ) x += q;
        if( verbose() ) std::cout << pre << "Truncating at nonconvexity in iteration " << step << ": " << qAq << std::endl;
        result = Result::TruncatedAtNonConvexity;
        return true;
      }

      if( Impl == CGImplementationType::TRUNCATED_REGULARIZED || Impl == CGImplementationType::REGULARIZED )
      {
        this->updateRegularization(qAq,qPq);
        if( verbose() ) std::cout << pre << "Regularizing at nonconvexity in iteration " << step << "." << std::endl;
        result = Result::EncounteredNonConvexity;
        return true;
      }
    }

    Operator A_;
    Operator P_;
    std::unique_ptr< CGTerminationCriterion > terminate = nullptr;
    mutable Result result = Result::Failed; ///< information about reason for termination
    mutable double energyNorm2 = 0.; ///< energy norm squared
    std::string pre = std::string("Algorithm CG: "); ///< output
  };
}
#endif // ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
