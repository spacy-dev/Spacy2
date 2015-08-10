#include "conjugateGradients.hh"

#include "functionSpaceElement.hh"
#include "Util/Exceptions/singularOperatorException.hh"

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>


namespace Algorithm
{
  FunctionSpaceElement CGMethod::solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b, double tolerance) const
  {
    terminate->setRelativeAccuracy(tolerance);
    return solve(x,b);
  }

  FunctionSpaceElement CGMethod::solve(const FunctionSpaceElement& b) const
  {
    auto x = FunctionSpaceElement( A_.impl().domain().element() );
    return solve(x,b);
  }

  FunctionSpaceElement CGMethod::solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b) const
  {
    initializeRegularization();
    nonconvexity = Nonconvexity::None;
    if( type_ == "cg" || type_ == "tcg" )
      return cgLoop(x,b);
    else
    {
      FunctionSpaceElement y = x;
      while( result != Result::Converged && result != Result::TruncatedAtNonConvexity )
        y = cgLoop(x,b);
      return y;
    }
  }

  CGTerminationCriterion& CGMethod::terminationCriterion() noexcept
  {
    assert(terminate!=nullptr);
    return *terminate;
  }

  bool CGMethod::encounteredNonConvexity() const noexcept
  {
    return nonconvexity == Nonconvexity::Encountered;
  }

  auto CGMethod::getEnergyNormOfSolution() const noexcept
  {
    return sqrt(energyNorm2);
  }

  FunctionSpaceElement CGMethod::cgLoop (FunctionSpaceElement x, FunctionSpaceElement r) const
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
      regularize(qAq,qPq);

      auto alpha = sigma/qAq;
      if( verbose_detailed() ) std::cout << "  sigma = " << sigma << ", alpha = " << alpha << ", qAq = " << qAq << ", qPq = " << qPq << std::endl;

      terminate->provideAlgorithmicQuantities(alpha,qAq,qPq,sigma);

      //  don't trust small numbers
      if( vanishingStep(step) )
      {
        result = Result::Converged;
        break;
      }


      if( terminateOnNonconvexity(qAq,qPq,x,q,step) ) break;

      x += alpha * q;
      energyNorm2 += alpha*qAq;
      if( verbose_detailed() ) std::cout << "  |x|^2_A = " << energyNorm2 << std::endl;

      // convergence test
      if (*terminate)
      {
        if( verbose() ) std::cout << type_ << ": Terminating in iteration " << step << ".\n";
        result = terminate->reachedMaximalNumberOfIterations() ? Result::Failed : Result::Converged;
        break;
      }

      r -= alpha*Aq;
      adjustRegularizedResidual(alpha,Pq,r);

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


  FunctionSpaceElement CGMethod::Q(const FunctionSpaceElement& r) const
  {
    auto Qr = P_(r);
    for(auto i=0u; i<iterativeRefinements(); ++i)
      Qr += P_(r-A_(Qr));
    return Qr;
  }

  bool CGMethod::vanishingStep(unsigned step) const
  {
    if( terminate->vanishingStep() )
    {
      if( verbose() ) std::cout << type_ << ": Terminating due to numerically almost vanishing step in iteration " << step << "." << std::endl;
      result = Result::Converged;
      return true;
    }
    return false;
  }

  bool CGMethod::terminateOnNonconvexity(double qAq, double qPq, FunctionSpaceElement& x, const FunctionSpaceElement& q, unsigned step) const
  {
    if( qAq > 0 ) return false;
    if( verbose() ) std::cout << type_ << ": Negative curvature: " << qAq << std::endl;

    if( type_ == "cg" )
    {
      if( verbose() )
      {
        std::cout << type_ << ": Direction of negative curvature encountered in standard CG Implementation!" << std::endl;
        std::cout << type_ << ": Either something is wrong with your operator or you should use TCG, RCG or HCG. Terminating CG!" << std::endl;
      }

      throw SingularOperatorException("CG::terminateOnNonconvexity");
    }

    if( type_ == "tcg" || ( type_ == "trcg" && terminate->minimalDecreaseAchieved() ) )
    {
      // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is encountered in the first step something probably went wrong
      // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
      if( step == 1 ) x += q;
      if( verbose() ) std::cout << type_ << ": Truncating at nonconvexity in iteration " << step << ": " << qAq << std::endl;
      nonconvexity = Nonconvexity::Encountered;
      result = Result::TruncatedAtNonConvexity;
      return true;
    }

    if( type_ == "trcg" || type_ == "rcg" )
    {
      updateRegularization(qAq,qPq);
      if( verbose() ) std::cout << type_ << ": Regularizing at nonconvexity in iteration " << step << "." << std::endl;
      nonconvexity = Nonconvexity::Encountered;
      result = Result::EncounteredNonConvexity;
      return true;
    }

    assert(false);
    return false;
  }

  void CGMethod::initializeRegularization() const noexcept
  {
    if( type_ == "cg" || type_ == "tcg" ) return;
    theta = 0;
  }

  void CGMethod::regularize(double& qAq, double qPq) const noexcept
  {
    if( type_ == "cg" || type_ == "tcg" ) return;
    qAq += theta*qPq;
  }

  void CGMethod::updateRegularization(double qAq, double qPq) const
  {
    if( type_ == "cg" || type_ == "tcg" ) return;
    double oldTheta = theta > 0 ? theta : eps();
    theta += (1-qAq)/std::abs(qPq);
    if( verbose() ) std::cout << "Computed regularization parameter: " << theta << std::endl;
    theta = std::min(std::max(minIncrease*oldTheta,theta),maxIncrease*oldTheta);
    if( verbose() ) std::cout << "Updating regularization parameter from " << oldTheta << " to " << theta << std::endl;
  }

  void CGMethod::adjustRegularizedResidual(double alpha, const FunctionSpaceElement& Pq, FunctionSpaceElement& r) const
  {
    if( type_ == "cg" || type_ == "tcg" ) return;
    r -= (alpha*theta)*Pq;
  }
}
