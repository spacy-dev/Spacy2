#ifndef SPACY_ACR_HH
#define SPACY_ACR_HH

#include <string>
#include <tuple>

#include "Spacy/linearSolver.hh"
#include "Spacy/functional.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Algorithm/lipschitzConstant.hh"
#include "Spacy/Algorithm/CompositeStep/quadraticModel.hh"
#include "Spacy/Util/mixins.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{                           
  /// \cond
 // namespace ACR{ class CubicModel; }
  /// \endcond
  

 namespace ACR
  {
    /**
     * @brief The affine covariant step method described in @cite Lubkoll2015, @cite Lubkoll2015a for the solution of equality constraint optimization problems.
     *
     * An affine covariant composite step method for the solution of problems of the form
     * \f[\min f(x)\quad \text{s.t.}\quad c(x)=0\f], based on the corresponding Lagrange functional
     * \f[L(x,p) = f(x)+pc(x)\f].
     */
    class ACRSolver :
        public Mixin::RegularityTest ,
        public Mixin::Timer<std::chrono::milliseconds> ,
        public Mixin::DecreaseCondition ,
        public Mixin::Eps ,
        public Mixin::MaxSteps ,
        public Mixin::MinimalAccuracy ,
        public Mixin::RelativeAccuracy ,
        public Mixin::Verbosity ,
        public Mixin::IterativeRefinements
    {
      enum class StepMonitor { Rejected , Accepted };
      enum class AcceptanceTest;

    public:
      /**
       * @brief Constructor.
       * @param N Lagrange functional for the problem \f[\min \|\delta x_k\| \quad \text{s.t.} c'(x_k)\delta x_k + c(x_k)=0\f]
       * @param L Lagrange functional
       * @param domain domain space \f$X=\{Y,U,P\}\f$
       */
      ACRSolver(C2Functional f);

      /// Compute solution.
      Vector operator()();

      /**
       * @brief Compute solution.
       * @param x0 initial iterate
       */
      Vector operator()(const Vector& x0);

    private:
 
      class TrivialPreconditioner
      {
		  public:
		  // copy constructor
	         	  
		     TrivialPreconditioner() {};
		     
			TrivialPreconditioner(const TrivialPreconditioner& p) {};
			//move constructor
			TrivialPreconditioner(TrivialPreconditioner&& p) {};
			//compute A(x)
			Vector operator()(const Vector& x) const{ return x; };
	  };
 
      C2Functional f_;
      const VectorSpace& domain_;
      StepMonitor stepMonitor = StepMonitor::Accepted;
      Vector computeStep(const Vector &x) const;
      StepMonitor acceptanceTest(const Vector &x, const Vector &dx, Real lambda, const CompositeStep::CubicModel& cubicModel) const;
      Real weightChange(Spacy::Vector dx, Spacy::Vector x, Spacy::Real omega_) const;

      Real omega = {1e-6};

      mutable IndefiniteLinearSolver tangentialSolver = {};

      std::string spacing = "  ", spacing2 = "    ";

		Real eta1,eta2,gamma1,gamma2,rho;
      Real norm_dx_old = -1;
    };
  }
}

#endif // SPACY_ACR_HH
