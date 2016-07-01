// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_AFFINECOVARIANTCOMPOSITESTEPS_HH
#define SPACY_AFFINECOVARIANTCOMPOSITESTEPS_HH

#include <string>
#include <tuple>

#include "Spacy/linearSolver.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Algorithm/lipschitzConstant.hh"
#include "Spacy/Util/mixins.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /// \cond
  namespace CompositeStep{ class CubicModel; }
  /// \endcond

  namespace CompositeStep
  {
    /**
     * @ingroup CSGroup
     * @brief The affine covariant step method described in @cite Lubkoll2015, @cite Lubkoll2015a for the solution of equality constraint optimization problems.
     *
     * An affine covariant composite step method for the solution of problems of the form
     * \f[\min f(x)\quad \text{s.t.}\quad c(x)=0\f], based on the corresponding Lagrange functional
     * \f[L(x,p) = f(x)+pc(x)\f].
     */
    class AffineCovariantSolver :
        public Mixin::RegularityTest ,
        public Mixin::Timer<std::chrono::milliseconds> ,
        public Mixin::AdjointIndex ,
        public Mixin::ControlIndex ,
        public Mixin::StateIndex ,
        public Mixin::ContractionRate ,
        public Mixin::DecreaseCondition ,
        public Mixin::Eps ,
        public Mixin::MaxSteps ,
        public Mixin::DampingAccuracy ,
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
      AffineCovariantSolver(C2Functional N, C2Functional L, VectorSpace& domain);

      AffineCovariantSolver(C2Functional N, C2Functional L, VectorSpace& domain, std::function<Vector(const Vector&, const Vector&) > retraction);

      AffineCovariantSolver(C2Functional N, C2Functional L, 
		VectorSpace& totalSpace, VectorSpace& chartSpace, 
		std::function<Vector(const Vector&, const Vector&) > retraction, 
		std::function<Vector(const Vector&, const Vector&) > dualUpdate);

      /// Compute solution.
      Vector operator()();

      /**
       * @brief Compute solution.
       * @param x0 initial iterate
       */
      Vector operator()(const Vector& x0);

    private:
      Vector computeNormalStep(const Vector& x) const;
      Vector computeSimplifiedNormalStep(const Vector& trial) const;
      Vector computeMinimumNormCorrection(const Vector& x) const;
      Vector computeTangentialStep(Real nu, const Vector& x, const Vector& dn, bool lastStepWasUndamped) const;

      Vector updateLagrangeMultiplier(const Vector& x) const;

      IndefiniteLinearSolver makeTangentialSolver(Real nu, const Vector& x, bool lastStepWasUndamped) const;

      bool convergenceTest(Real nu, Real tau, Real norm_x, Real norm_dx);

      std::function<Vector(const Vector&, const Vector&)> linearRetraction = [](const Vector& origin,const Vector& increment) { return origin+increment; };


      std::tuple<Real, Vector, Vector, Real, Real> computeCompositeStep(Real& nu, Real norm_Dn,
                  const Vector& x, const Vector& Dn, const Vector& Dt);

      void updateOmegaC(Real norm_x, Real norm_dx, Real norm_ds);
      Real updateOmegaL(const Vector& soc, Real q_tau,
                          Real tau, Real norm_x, Real norm_dx, const CompositeStep::CubicModel& cubic);

      Real computeNormalStepDampingFactor(Real norm_Dn) const;
      Real computeTangentialStepDampingFactor(Real norm_dn, Real norm_Dt, const CompositeStep::CubicModel& cubic) const;

      void regularityTest(Real nu, Real tau) const;
      AcceptanceTest acceptedSteps(Real norm_x, Real normDx, Real eta);

      Vector retractPrimal(const Vector& origin, const Vector& increment) const;

      std::function<Vector(const Vector&, const Vector&) > retraction_;
      std::function<Vector(const Vector&, const Vector&) > dualUpdate_;
      C2Functional N_, L_;
      VectorSpace& domain_;
      VectorSpace& chartSpace_;

      LipschitzConstant omegaL = {1e-6}, omegaC = {1e-6};

      mutable LinearSolver normalSolver = {};
      mutable IndefiniteLinearSolver tangentialSolver = {};


      std::string spacing = "  ", spacing2 = "    ";

      StepMonitor normalStepMonitor = StepMonitor::Accepted;
      StepMonitor tangentialStepMonitor = StepMonitor::Accepted;

      Real norm_dx_old = -1;
    };
  }
}

#endif // SPACY_AFFINECOVARIANTCOMPOSITESTEPS_HH
