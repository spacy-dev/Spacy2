#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <cassert>
#include <iostream>
#include <memory>

#include "../inverseOperator.hh"
#include "../FunctionSpaces/RealNumbers/realSolver.hh"
#include "../functionSpaceElement.hh"
#include "../norm.hh"
#include "../derivative.hh"
#include "../differentiableOperator.hh"
#include "../operator.hh"
#include "dampingStrategies.hh"

#include <functional>

namespace Algorithm
{
  class Newton
  {
  public:
    Newton(DifferentiableOperator& F, bool verbose = false)
      : F_(F),
        norm_(F_.getDomain().getNorm()),
        dampingFactor_(DampingStrategy::AffineCovariant<Newton>(*this)),
        verbose_(verbose)
    {}

    Newton(DifferentiableOperator& F, const std::function<double(InverseOperator&, const FunctionSpaceElement&, const FunctionSpaceElement&)>& dampingStrategy, bool verbose = false)
      : F_(F),
        norm_(F_.getDomain().getNorm()),
        dampingFactor_(dampingStrategy),
        verbose_(verbose)
    {}

    FunctionSpaceElement solve() const
    {
      auto x0 = F_.getDomain().element();
      return solve(x0);
    }

    FunctionSpaceElement solve(const FunctionSpaceElement& x0) const
    {
      if( verbose_ ) std::cout << "Starting newton iteration with initial guess: " << x0;

      Operator DF = derivative(F_);
      InverseOperator DFInv(std::make_shared<RealSolver>(DF));

      auto x = x0;
      for(unsigned i = 1; i <= maxIterations_; ++i)
      {
        if( verbose_ ) std::cout << "Iteration: " << i << ", ";

        auto dx = DFInv(-F_(x));

        if( norm_(dx) < relativeAccuracy_ * norm_(x) )
        {
          if( verbose_ ) std::cout << "Terminating (rel. acc.: " << norm_(dx)/norm_(x) << ")\n";
          return x;
        }

        x += dampingFactor_(DFInv,x,dx)*dx;
      }

      return x;
    }

    void setMaximalNumberOfIterations(unsigned maxIterations)
    {
      maxIterations_ = maxIterations;
    }

    void setRelativeAccuracy(double relativeAccuracy)
    {
      relativeAccuracy_ = relativeAccuracy;
    }

    void setThetaMax(double thetaMax)
    {
      assert(thetaMax > 0 && thetaMax < 1);
      thetaMax_ = thetaMax;
    }

    void setThetaAim(double thetaAim)
    {
      assert(thetaAim > 0 && thetaAim < 1);
      thetaAim_ = thetaAim;
    }

  private:
    friend class DampingStrategy::AffineCovariant<Newton>;
    DifferentiableOperator& F_;
    Norm norm_;
    std::function<double(InverseOperator&,const FunctionSpaceElement&,const FunctionSpaceElement&)> dampingFactor_;
    unsigned maxIterations_ = 10;
    double relativeAccuracy_ = 1e-6;
    double thetaMax_ = 0.75, thetaAim_ = 0.5;
    bool verbose_ = false;
  };
}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
