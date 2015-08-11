#ifndef ALGORITHM_ALGORITHM_COMPOSITE_STEP_QUADRATICMODEL_HH
#define ALGORITHM_ALGORITHM_COMPOSITE_STEP_QUADRATICMODEL_HH

#include <cassert>
#include "Algorithm/Functions_1D/quadratic.hh"

namespace Algorithm
{
  /// \cond
  class ScalarProduct;
  class Vector;
  class Functional;
  /// \endcond

  namespace CompositeStep
  {
    Functions_1D::Quadratic makeQuadraticModel(double nu,
                                      const Vector& dn, const Vector& dt,
                                      const Functional& f, const Vector& x);

    Functions_1D::Quadratic makeQuadraticNormModel(double nu, const Vector& dn, const Vector& dt);

    class CubicModel
    {
    public:
      CubicModel(const Functions_1D::Quadratic& quadraticModel, const Functions_1D::Quadratic& squaredNorm, double omega);

      double operator()(double t) const;

    private:
      Functions_1D::Quadratic quadraticModel_;
      Functions_1D::Quadratic squaredNorm_;
      double omega_;
    };

    CubicModel makeCubicModel(double nu, const Vector& dn, const Vector& dt,
                              const Functional& f, const Vector& x, double omega);

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
