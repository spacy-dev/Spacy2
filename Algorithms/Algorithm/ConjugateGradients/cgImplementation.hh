/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*  This file is part of the library KASKADE 7                               */
/*    see http://www.zib.de/en/numerik/software/kaskade-7.html               */
/*                                                                           */
/*  Copyright (C) 2002-2013 Zuse Institute Berlin                            */
/*                                                                           */
/*  KASKADE 7 is distributed under the terms of the ZIB Academic License.    */
/*    see $KASKADE/academic.txt                                              */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CG_ImplEMENTATION_HH
#define CG_ImplEMENTATION_HH

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>

#include "../c0Operator.hh"
#include "../functionSpaceElement.hh"
#include "../Util/singularoperatorexception.hh"

#include "cgTerminationCriteria.hh"

namespace Algorithm
{
  enum class CGImplementationType { STANDARD, TRUNCATED, REGULARIZED, HYBRID };

  namespace CG_Detail
  {
    struct NoRegularization
    {
      NoRegularization(double,bool) noexcept;

      void initializeRegularization() const noexcept;
      void regularize(double&, double) const noexcept;
      void updateRegularization(double,double) const noexcept;
      template <class X>
      void adjustRegularizedResidual(double, const X&, const X&) const noexcept {}
    };


    struct Regularization
    {
      Regularization(double eps, bool verbose) noexcept;

      void initializeRegularization() noexcept;

      void regularize(double& qAq, double qPq) const noexcept;

      void updateRegularization(double qAq, double qPq);

      template <class X>
      void adjustRegularizedResidual(double alpha, const X& Pq, X& r) const
      {
        r.axpy(-alpha*theta,Pq);
      }

    private:
      double theta = 0, eps_ = 1e-16;
      bool verbose_ = 0;
      unsigned maxIncrease = 1000, minIncrease = 2;
    };


    template <CGImplementationType Impl>
    using ChooseRegularization = typename std::conditional< ( Impl==CGImplementationType::STANDARD || Impl==CGImplementationType::TRUNCATED ),
    NoRegularization,
    Regularization >::type;
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
  class CGBase : public CG_Detail::ChooseRegularization<Impl>
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
    CGBase(const C0Operator& A, const C0Operator& P, bool verbose_ = true, double eps_ = 1e-12)
      : CG_Detail::ChooseRegularization<Impl>(eps_,verbose_),
        A_(A), P_(P), terminate(std::make_unique< RelativeEnergyError >()), verbose(verbose_)
    {
      initPre();
      terminate->setMaximalAttainableAccuracy(eps_);
    }

    /**
     * @param x initial guess
     * @param b right hand side
     * @param tolerance tolerance of the termination criterion
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b, double tolerance)
    {
      terminate->setTolerance(tolerance);
      return solve(x,b);
    }

    /**
     * @param x initial guess
     * @param b right hand side
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b)
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
    CGTerminationCriterion& getTerminationCriterion() const noexcept
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
    /// CG Implementation.
    FunctionSpaceElement cgLoop (FunctionSpaceElement x, FunctionSpaceElement r)
    {
      terminate->clear();
      result = Result::Failed;

      // initialization phase for conjugate gradients
      auto Ax = A_(x);
      r -= Ax;
      auto Qr = P_(r);

      auto q = Qr;
      auto Pq = r; // required only for regularized or hybrid conjugate gradient methods

      auto sigma = std::abs(Qr*r); // preconditioned residual norm squared

      // the conjugate gradient iteration
      for (unsigned step = 1; true; step++ )
      {
        auto Aq = A_(q);
        auto qAq = q*Aq;
        auto qPq = q*Pq;

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
          result = terminate->reachedMaximalNumberOfIterations() ? Result::Failed : Result::Converged;
          break;
        }

        r -= alpha*Aq;
        this->adjustRegularizedResidual(alpha,Pq,r);

        Qr = P_(r);

        // determine new search direction
        auto sigmaNew = std::abs(Qr*r); // sigma = <Qr,r>
        auto beta = sigmaNew/sigma;
        sigma = sigmaNew;

        q *= beta; q += Qr;  //  q = Qr + beta*q
        Pq *= beta; Pq += r; // Pq = r + beta*Pq
      }

      return x;
    }

    /// Set string pre.
    void initPre()
    {
      if(Impl == CGImplementationType::STANDARD) pre = std::string("KASKADE CG: ");
      if(Impl == CGImplementationType::TRUNCATED) pre = std::string("KASKADE TCG: ");
      if(Impl == CGImplementationType::REGULARIZED) pre = std::string("KASKADE RCG: ");
      if(Impl == CGImplementationType::HYBRID) pre = std::string("KASKADE HCG: ");
    }

    /// Check step length.
    bool vanishingStep()
    {
      if( terminate->vanishingStep() )
      {
        if( verbose ) std::cout << pre << "Terminating due to numerically almost vanishing step." << std::endl;
        result = Result::Converged;
        return true;
      }
      return false;
    }

    /**
     * \return true if iteration should terminate.
     */
    template <class Scalar>
    bool terminateOnNonconvexity(Scalar qAq, Scalar qPq, FunctionSpaceElement& x, const FunctionSpaceElement& q, unsigned step)
    {
      if( qAq > 0 ) return false;

      if( Impl == CGImplementationType::STANDARD )
      {
        if( verbose )
        {
          std::cout << pre << "Direction of negative curvature encountered in standard CG Implementation!" << std::endl;
          std::cout << pre << "Either something is wrong with your operator or you should use TCG, RCG or HCG. Terminating CG!" << std::endl;
        }

        throw SingularOperatorException("CG::terminateOnNonconvexity");
        //            result = Result::EncounteredNonConvexity;
        //            return true;
      }

      if( Impl == CGImplementationType::TRUNCATED || ( Impl == CGImplementationType::HYBRID && terminate->minimalDecreaseAchieved() ) )
      {
        // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is encountered in the first step something probably went wrong
        // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
        if( step == 1 ) x += q;
        if( verbose ) std::cout << pre << "Truncating at nonconvexity in iteration " << step << ": " << qAq << std::endl;
        result = Result::TruncatedAtNonConvexity;
        return true;
      }

      if( Impl == CGImplementationType::HYBRID || Impl == CGImplementationType::REGULARIZED )
      {
        this->updateRegularization(qAq,qPq);
        if( verbose ) std::cout << pre << "Regularizing at nonconvexity in iteration " << step << "." << std::endl;
        result = Result::EncounteredNonConvexity;
        return true;
      }
    }

    const C0Operator& A_;
    const C0Operator& P_;
    std::unique_ptr< CGTerminationCriterion > terminate = nullptr;
    bool verbose = false; ///
    Result result = Result::Failed; ///< information on the way cg did terminate
    double energyNorm2 = 0.; ///< energy norm squared
    std::string pre = std::string("Algorithm CG: "); ///< output
  };
} // namespace Kaskade
#endif
