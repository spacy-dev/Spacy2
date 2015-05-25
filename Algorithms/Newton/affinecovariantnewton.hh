#ifndef AFFINECOVARIANTNEWTON_HH
#define AFFINECOVARIANTNEWTON_HH

#include <iostream>
#include <memory>

#include "../inverseOperator.hh"
#include "../FunctionSpaces/RealNumbers/realSolver.hh"
#include "../functionSpaceElement.hh"
#include "../norm.hh"
#include "../operator.hh"
#include "dampingStrategies.hh"

#include <functional>

namespace Algorithm
{
  class Newton
  {
  public:
    Newton(Operator& F, bool verbose = false)
      : F_(F), norm_(F_.getDomain().getNorm()), FInv_(std::make_unique<RealSolver>(F_)),
        dampingFactor_(DampingStrategy::AffineCovariant<Newton>(*this)),
        verbose_(verbose)
    {}

    Newton(Operator& F, const std::function<double(const FunctionSpaceElement&, const FunctionSpaceElement&)>& dampingStrategy, bool verbose = false)
      : F_(F), norm_(F_.getDomain().getNorm()), FInv_(std::make_unique<RealSolver>(F_)),
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
      auto x = x0;
      for(unsigned i = 1; i <= maxIterations_; ++i)
      {
        if( verbose_ ) std::cout << "Iteration: " << i << ", ";

        auto dx = FInv_(x,-F_(x));

        if( norm_(dx) < relativeAccuracy_ * norm_(x) )
        {
          if( verbose_ ) std::cout << "Terminating (rel. acc.: " << norm_(dx)/norm_(x) << ")\n";
          return x;
        }

        x += dampingFactor_(x,dx)*dx;
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
    Operator& F_;
    Norm norm_;
    InverseOperator FInv_;
    std::function<double(const FunctionSpaceElement&,const FunctionSpaceElement&)> dampingFactor_;
    unsigned maxIterations_ = 1000;
    double relativeAccuracy_ = 1e-6;
    double thetaMax_ = 0.75, thetaAim_ = 0.5;
    bool verbose_ = false;
  };

  auto makeNewton(Operator& F, bool verbose = false)
  {
    return Newton(F,verbose);
  }
}
#endif // AFFINECOVARIANTNEWTON_HH
