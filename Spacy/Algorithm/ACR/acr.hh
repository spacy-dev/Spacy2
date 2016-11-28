#pragma once

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
#include <iomanip> 

namespace Spacy
{                           
  /// \cond
 // namespace ACR{ class CubicModel; }
  /// \endcond
  

 namespace ACR
  {
    /**
     * @brief An ACR solver for solving unconstrained minimization problems 
     *
     * An ACR solver for solving problems of the form
     * \f[\min f(x)\f], 
     * 
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
       * @param f C2Functional to minimize
       */
      ACRSolver(C2Functional f,  double eta1 = 0.25,  double eta2 = 0.5, double epsilon = 1e-4, double relativeAccuracy = 1e-4, double omegaMax = 1e8);

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
		     
			TrivialPreconditioner(const TrivialPreconditioner& p) = default;
			//move constructor
			TrivialPreconditioner(TrivialPreconditioner&& p)  = default;
			//compute A(x)
			Vector operator()(const Vector& x) const{ return x; };
      };
 
      C2Functional f_;
      const VectorSpace& domain_;
      StepMonitor stepMonitor = StepMonitor::Accepted;
      Vector computeStep(const Vector &x) const;
      
      StepMonitor acceptanceTest(const Vector &x, const Vector &dx, const Real & lambda, const CompositeStep::CubicModel& cubicModel);
      Real weightChange(::Spacy::Vector dx, ::Spacy::Vector x, ::Spacy::Real omega) const;
   
      mutable LinearSolver preconditioner = {};

      const double eta1_, eta2_,  epsilon_,  relativeAccuracy_,  omegaMax_;
      Real rho_ = { 1.0 };
      Real omega_ = { 1e-6 };

    };
  }
}

