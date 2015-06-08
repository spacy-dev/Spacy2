#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <cassert>
#include <iostream>
#include <memory>

#include "linearOperator.hh"
#include "FunctionSpaces/RealNumbers/realSolver.hh"
#include "functionSpaceElement.hh"
#include "norm.hh"
#include "derivative.hh"
#include "c1Operator.hh"
#include "operator.hh"
#include "dampingStrategies.hh"

#include <functional>

namespace Algorithm
{
  class Newton
  {
  public:
    Newton(C1Operator& F, bool verbose = false)
      : F_(F),
        dampingFactor_(DampingStrategy::AffineCovariant<Newton>(*this)),
        verbose_(verbose)
    {}

    Newton(C1Operator& F, const std::function<double(const LinearSolver&, const FunctionSpaceElement&, const FunctionSpaceElement&)>& dampingStrategy, bool verbose = false)
      : F_(F),
        dampingFactor_(dampingStrategy),
        verbose_(verbose)
    {}

//    FunctionSpaceElement solve() const
//    {
//      auto x0 = F_.getDomain().element();
//      return solve(x0);
//    }

    FunctionSpaceElement solve(const FunctionSpaceElement& x0)
    {
      if( verbose_ ) std::cout << "Starting newton iteration with initial guess: " << x0;

      norm_ = Norm( x0.impl().getSpace().getNorm() );

      auto x = x0;
      for(unsigned i = 1; i <= maxIterations_; ++i)
      {
        if( verbose_ ) std::cout << "Iteration: " << i << ", ";

        F_.setArgument(x);
        LinearSolver DFInv(F_.getLinearization().getSolver());

        auto dx = DFInv(-F_());

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
    C1Operator& F_;
    std::function<double(const LinearSolver&,const FunctionSpaceElement&,const FunctionSpaceElement&)> dampingFactor_;
    unsigned maxIterations_ = 10;
    double relativeAccuracy_ = 1e-6;
    double thetaMax_ = 0.75, thetaAim_ = 0.5;
    bool verbose_ = false;
    Norm norm_;
  };
}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
