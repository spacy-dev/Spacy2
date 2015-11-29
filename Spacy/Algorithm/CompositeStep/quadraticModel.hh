// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_SPACY_COMPOSITE_STEP_QUADRATICMODEL_HH
#define SPACY_SPACY_COMPOSITE_STEP_QUADRATICMODEL_HH

#include <cassert>

#include "Spacy/Algorithm/Functions_1D/quadratic.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /// @cond
  class C2Functional;
  class Vector;
  /// @endcond

  /** @addtogroup CSGroup
   * {
   */

  namespace CompositeStep
  {
    /**
     * @brief Constructs the quadratic model of a Lagrange functional for a composite step method.
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @param L Lagrange functional
     * @param x current iterate
     *
     * Constructs a quadratic function \f$ q(t)=a+bt+ct^2\f$, with
     * - \f$ a = L(x)+\nu L'(x)dn+\frac{\nu^2}{2}L''(x)(dn)^2 \f$
     * - \f$ b = L'(x)dt + \nu L''(x)(dn,dt) \f$
     * - \f$ c = \frac{1}{2}L''(x)(dt)^2 \f$
     *
     * @return \f$ q(t)=a+bt+ct^2\f$
     */
    Functions_1D::Quadratic makeQuadraticModel(Real nu,
                                      const Vector& dn, const Vector& dt,
                                      const C2Functional& L, const Vector& x);

    /**
     * @brief Constructs the quadratic model a norm for a composite step method.
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @return \f$ q(t)=\nu^2\|dn\|^2+2\nu(dn,dt)t+\|dt\|^2t^2 \f$
     */
    Functions_1D::Quadratic makeQuadraticNormModel(Real nu, const Vector& dn, const Vector& dt);

    /// The cubic regularized model for AffineCovariantCompositeSteps.
    class CubicModel
    {
    public:
      /**
       * @brief Constructor.
       * @param quadraticModel quadratic model \f$q_1\f$ of the Lagrange functional (generated with makeQuadraticModel(...))
       * @param squaredNorm quadratic model \f$q_2\f$ of a norm (generated with makeQuadraticNormModel(...))
       * @param omega estimate of the Lipschitz constant of the second derivative of the Lagrangian
       */
      CubicModel(const Functions_1D::Quadratic& quadraticModel, const Functions_1D::Quadratic& squaredNorm, Real omega);

      /**
       * @brief Evaluate cubic model \f$ q(t) = q_1(t) + \frac{\omega}{6}q_2^{3/2} \f$.
       * @param t argument
       * @return \f$ q(t) = q_1(t) + \frac{\omega}{6}q_2^{3/2} \f$
       */
      Real operator()(Real t) const;

    private:
      Functions_1D::Quadratic quadraticModel_;
      Functions_1D::Quadratic squaredNorm_;
      Real omega_;
    };

    /**
     * @brief Generate cubic regularized model for AffineCovariantCompositeSteps
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @param L Lagrange functional
     * @param x current iterate
     * @param omega estimate of the Lipschitz constant of the second derivative of the Lagrangian
     * @return CubicModel( makeQuadraticModel(nu,dn,dt,L,x), makeQuadraticNormModel(nu,dn,dt), omega )
     */
    CubicModel makeCubicModel(Real nu, const Vector& dn, const Vector& dt,
                              const C2Functional& L, const Vector& x, Real omega);

    /**
     * @brief Find global minimizer of \f$f\f$ in \f$[a,b]\f$.
     * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
     * @param a lower bound of the admissible interval
     * @param b upper bound of the admissible interval
     * @param eps relative accuracy
     * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$
     */
    template <class Model>
    Real findMinimizer(const Model& f, Real a, Real b, Real eps = 1e-2)
    {
      assert(a<b);
      eps *= b-a;
      Real t = a;
      Real tmin = t;
      Real fmin = f(t);

      while( (t+=eps) <= b)
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

  /** @} */
}

#endif // SPACY_SPACY_COMPOSITE_STEP_QUADRATICMODEL_HH
