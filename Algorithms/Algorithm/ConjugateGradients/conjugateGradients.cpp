#include "conjugateGradients.hh"

#include "vector.hh"
#include "Util/Exceptions/singularOperatorException.hh"

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>


namespace Algorithm
{
  CGMethod::CGMethod(CallableOperator A, CallableOperator P, const std::string& type)
    : A_(std::move(A)), P_(std::move(P)),
      terminate(std::make_shared< RelativeEnergyError >()), type_(type)
  {
    attachEps(terminate.get());
    attachAbsoluteAccuracy(terminate.get());
    attachRelativeAccuracy(terminate.get());
    attachVerbosity(terminate.get());
    attachMaxSteps(terminate.get());
    assert( type=="CG" || type=="RCG" || type=="TCG" || type=="TRCG" );
  }

  Vector CGMethod::solve(const Vector& x, const Vector& b, double tolerance) const
  {
    terminate->setRelativeAccuracy(tolerance);
    return solve(x,b);
  }

  Vector CGMethod::solve(const Vector& x, const Vector& b) const
  {
    initializeRegularization();
    nonconvexity = Nonconvexity::None;
    result = Result::Failed;
    if( type_ == "CG" || type_ == "TCG" )
      return cgLoop(x,b);
    else
    {
      Vector y = x;
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

  const CallableOperator& CGMethod::P() const
  {
    return P_;
  }

  const CallableOperator& CGMethod::A() const
  {
    return A_;
  }

  Vector CGMethod::cgLoop (Vector x, Vector r) const
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
      if( verbose_detailed() ) std::cout << "Iteration: " << step << std::endl;
      auto Aq = A_(q);
      auto qAq = Aq(q);
      auto qPq = Pq(q);
      regularize(qAq,qPq);

      auto alpha = sigma/qAq;
      if( verbose_detailed() ) std::cout << "    " << type_ << "  sigma = " << sigma << ", alpha = " << alpha << ", qAq = " << qAq << ", qPq = " << qPq << std::endl;

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
      if( verbose_detailed() ) std::cout << "    " << type_ << "  |x|^2_A = " << energyNorm2 << std::endl;

      // convergence test
      if (*terminate)
      {
        if( verbose() ) std::cout << "    " << type_ << ": Terminating in iteration " << step << ".\n";
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


  Vector CGMethod::Q(const Vector& r) const
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
      if( verbose() ) std::cout << "    " << type_ << ": Terminating due to numerically almost vanishing step in iteration " << step << "." << std::endl;
      result = Result::Converged;
      return true;
    }
    return false;
  }

  bool CGMethod::terminateOnNonconvexity(double qAq, double qPq, Vector& x, const Vector& q, unsigned step) const
  {
    if( qAq > 0 ) return false;
    if( verbose() ) std::cout << "    " << type_ << ": Negative curvature: " << qAq << std::endl;

    if( type_ == "CG" )
    {
      if( verbose() )
      {
        std::cout << "    " << type_ << ": Direction of negative curvature encountered in standard CG Implementation!" << std::endl;
        std::cout << "    " << type_ << ": Either something is wrong with your operator or you should use TCG, RCG or HCG. Terminating CG!" << std::endl;
      }

      throw SingularOperatorException("CG::terminateOnNonconvexity");
    }

    if( type_ == "TCG" || ( type_ == "TRCG" && terminate->minimalDecreaseAchieved() ) )
    {
      // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is encountered in the first step something probably went wrong
      // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
      if( step == 1 ) x += q;
      if( verbose() ) std::cout << "    " << type_ << ": Truncating at nonconvexity in iteration " << step << ": " << qAq << std::endl;
      nonconvexity = Nonconvexity::Encountered;
      result = Result::TruncatedAtNonConvexity;
      return true;
    }

    if( type_ == "TRCG" || type_ == "RCG" )
    {
      updateRegularization(qAq,qPq);
      if( verbose() ) std::cout << "    " << type_ << ": Regularizing at nonconvexity in iteration " << step << "." << std::endl;
      nonconvexity = Nonconvexity::Encountered;
      result = Result::EncounteredNonConvexity;
      return true;
    }

    assert(false);
    return false;
  }

  void CGMethod::initializeRegularization() const noexcept
  {
    if( type_ == "CG" || type_ == "TCG" ) return;
    theta = 0;
  }

  void CGMethod::regularize(double& qAq, double qPq) const noexcept
  {
    if( type_ == "CG" || type_ == "TCG" ) return;
    qAq += theta*qPq;
  }

  void CGMethod::updateRegularization(double qAq, double qPq) const
  {
    if( type_ == "CG" || type_ == "TCG" ) return;
    double oldTheta = theta > 0 ? theta : eps();
    theta += (1-qAq)/std::abs(qPq);
    if( verbose() ) std::cout << "Computed regularization parameter: " << theta << std::endl;
    theta = std::min(std::max(minIncrease*oldTheta,theta),maxIncrease*oldTheta);
    if( verbose() ) std::cout << "Updating regularization parameter from " << oldTheta << " to " << theta << std::endl;
  }

  void CGMethod::adjustRegularizedResidual(double alpha, const Vector& Pq, Vector& r) const
  {
    if( type_ == "CG" || type_ == "TCG" ) return;
    r -= (alpha*theta)*Pq;
  }
}
