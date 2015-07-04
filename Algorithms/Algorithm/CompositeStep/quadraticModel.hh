#ifndef ALGORITHM_ALGORITHM_COMPOSITE_STEP_QUADRATICMODEL_HH
#define ALGORITHM_ALGORITHM_COMPOSITE_STEP_QUADRATICMODEL_HH

#include <cassert>
#include <limits>

#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  class ScalarProduct;
  class FunctionSpaceElement;
  class LagrangeFunctional;

  namespace CompositeStep
  {
    class QuadraticModel
    {
    public:
      QuadraticModel(double constant, double linear, double quadratic);

      double operator()(double t) const;

    private:
      double constant_, linear_, quadratic_;
    };

    QuadraticModel makeQuadraticModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const LagrangeFunctional& f, const FunctionSpaceElement& x);

    QuadraticModel makeQuadraticNormModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp);

    class CubicModel
    {
    public:
      CubicModel(const QuadraticModel& quadraticModel, const QuadraticModel& squaredNorm, double omega);

      double operator()(double t) const;

    private:
      QuadraticModel quadraticModel_;
      QuadraticModel squaredNorm_;
      double omega_;
    };

    CubicModel makeCubicModel(double nu, const FunctionSpaceElement& dn, const FunctionSpaceElement& dt, const ScalarProduct& sp, const LagrangeFunctional& f, const FunctionSpaceElement& x, double omega);

    template <class Model>
    double findMinimizer(const Model& f, double a, double b, double eps = 1e-2)
    {
      assert(a<b);
      double t = a;
      double tmin = t;
      double fmin = f(t);

      while( (t+=eps) < b)
      {
        if( f(t) < fmin )
        {
          fmin = f(t);
          tmin = t;
        }
      }

      return tmin;
    }
  }
}

#endif // ALGORITHM_ALGORITHM_COMPOSITE_STEP_QUADRATICMODEL_HH
