#include "cg.hh"

#include "VSA/vector.hh"
#include "VSA/Util/Exceptions/singularOperatorException.hh"

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>


namespace VSA
{
  namespace CG
  {
    Solver::Solver(CallableOperator A, CallableOperator P, const std::string& type)
      : A_(std::move(A)), P_(std::move(P)),
        terminate( CG::Termination::StrakosTichyEnergyError{} ), type_(type)
    {
  //    attachEps(terminate);
  //    attachAbsoluteAccuracy(terminate);
  //    attachRelativeAccuracy(terminate);
      assert( type=="CG" || type=="RCG" || type=="TCG" || type=="TRCG" );
    }

    Vector Solver::solve(const Vector& x, const Vector& b) const
    {
      initializeRegularization();
      definiteness_ = DefiniteNess::PositiveDefinite;
      result = Result::Failed;

      terminate.setEps(eps());
      terminate.setRelativeAccuracy(relativeAccuracy());
      terminate.setAbsoluteAccuracy(absoluteAccuracy());

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

//    CG::TerminationCriterion& Solver::terminationCriterion() noexcept
//    {
//      return terminate;
//    }

    bool Solver::indefiniteOperator() const noexcept
    {
      return definiteness_ == DefiniteNess::Indefinite;
    }

//    auto Solver::getEnergyNormOfSolution() const noexcept
//    {
//      return sqrt(energyNorm2);
//    }

    const CallableOperator& Solver::P() const
    {
      return P_;
    }

    const CallableOperator& Solver::A() const
    {
      return A_;
    }

    Vector Solver::cgLoop (Vector x, Vector r) const
    {
      terminate.clear();
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
        if( verbosityLevel() > 1 ) std::cout << "Iteration: " << step << std::endl;
        auto Aq = A_(q);
        auto qAq = Aq(q);
        auto qPq = Pq(q);
        regularize(qAq,qPq);

        auto alpha = sigma/qAq;
        if( verbosityLevel() > 1 ) std::cout << "    " << type_ << "  sigma = " << sigma << ", alpha = " << alpha << ", qAq = " << qAq << ", qPq = " << qPq << std::endl;

        terminate.update(alpha,qAq,qPq,sigma);

        //  don't trust small numbers
        if( vanishingStep(step) )
        {
          result = Result::Converged;
          break;
        }


        if( terminateOnNonconvexity(qAq,qPq,x,q,step) ) break;

        x += alpha * q;
        energyNorm2 += alpha*qAq;
        if( verbosityLevel() > 1 ) std::cout << "    " << type_ << "  |x|^2_A = " << energyNorm2 << std::endl;

        // convergence test
        if (terminate())
        {
          if( verbose() ) std::cout << "    " << type_ << ": Terminating in iteration " << step << ".\n";
          result = (step == maxSteps()) ? Result::Failed : Result::Converged;
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


    Vector Solver::Q(const Vector& r) const
    {
      auto Qr = P_(r);
      for(auto i=0u; i<iterativeRefinements(); ++i)
        Qr += P_(r-A_(Qr));
      return Qr;
    }

    bool Solver::vanishingStep(unsigned step) const
    {
      if( terminate.vanishingStep() )
      {
        if( verbose() ) std::cout << "    " << type_ << ": Terminating due to numerically almost vanishing step in iteration " << step << "." << std::endl;
        result = Result::Converged;
        return true;
      }
      return false;
    }

    bool Solver::terminateOnNonconvexity(double qAq, double qPq, Vector& x, const Vector& q, unsigned step) const
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

      if( type_ == "TCG" || ( type_ == "TRCG" && terminate.minimalDecreaseAchieved() ) )
      {
        // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is encountered in the first step something probably went wrong
        // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
        if( step == 1 ) x += q;
        if( verbose() ) std::cout << "    " << type_ << ": Truncating at nonconvexity in iteration " << step << ": " << qAq << std::endl;
        definiteness_ = DefiniteNess::Indefinite;
        result = Result::TruncatedAtNonConvexity;
        return true;
      }

      if( type_ == "TRCG" || type_ == "RCG" )
      {
        updateRegularization(qAq,qPq);
        if( verbose() ) std::cout << "    " << type_ << ": Regularizing at nonconvexity in iteration " << step << "." << std::endl;
        definiteness_ = DefiniteNess::Indefinite;
        result = Result::EncounteredNonConvexity;
        return true;
      }

      assert(false);
      return false;
    }

    void Solver::initializeRegularization() const noexcept
    {
      if( type_ == "CG" || type_ == "TCG" ) return;
      theta = 0;
    }

    void Solver::regularize(double& qAq, double qPq) const noexcept
    {
      if( type_ == "CG" || type_ == "TCG" ) return;
      qAq += theta*qPq;
    }

    void Solver::updateRegularization(double qAq, double qPq) const
    {
      if( type_ == "CG" || type_ == "TCG" ) return;
      double oldTheta = theta > 0 ? theta : eps();
      theta += (1-qAq)/std::abs(qPq);
      if( verbosityLevel() > 1 ) std::cout << "Computed regularization parameter: " << theta << std::endl;
      theta = std::min(std::max(minIncrease*oldTheta,theta),maxIncrease*oldTheta);
      if( verbosityLevel() > 1 ) std::cout << "Updating regularization parameter from " << oldTheta << " to " << theta << std::endl;
    }

    void Solver::adjustRegularizedResidual(double alpha, const Vector& Pq, Vector& r) const
    {
      if( type_ == "CG" || type_ == "TCG" ) return;
      r -= (alpha*theta)*Pq;
    }
  }
}
