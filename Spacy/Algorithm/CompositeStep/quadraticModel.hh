#pragma once

#include <cassert>

#include <Spacy/Algorithm/Scalar/quadratic.hh>
#include <Spacy/Algorithm/dampingFactor.hh>

namespace Spacy
{
  /// @cond
  class C2Functional;
  class Vector;
  /// @endcond

  /** @addtogroup CSGroup @{ */
  namespace CompositeStep
  {
    /**
     * @brief Constructs a quadratic model of a functional.
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @param x current iterate
     *
     * Constructs a quadratic function \f$ q(t)=a+bt+ct^2\f$, with
     * - \f$ a = L(x)+\nu L'(x)dn+\frac{\nu^2}{2}L''(x)(dn)^2 \f$
     * - \f$ b = L'(x)dt + \nu L''(x)(dn,dt) \f$
     * - \f$ c = \frac{1}{2}L''(x)(dt)^2 \f$
     *
     * @return \f$ q(t)=a+bt+ct^2\f$
     */
    Quadratic makeQuadraticModel(DampingFactor nu,
                                 const Vector& dn, const Vector& dt,
                                 const C2Functional& L, const Vector& x);

    /**
     * @brief Constructs a quadratic model for the norm of the underlying vector space.
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @return \f$ q(t)=\nu^2\|dn\|^2+2\nu(dn,dt)t+\|dt\|^2t^2 \f$
     */
    Quadratic makeQuadraticNormModel(DampingFactor nu, const Vector& dn, const Vector& dt);

    /// The cubic regularized model for the affine covariant composite step method of \cite Lubkoll2015, \cite Lubkoll2015a.
    class CubicModel
    {
    public:
      /**
       * @brief Constructor.
       * @param quadraticModel quadratic model \f$q_1\f$ of the Lagrange functional (generated with makeQuadraticModel(...))
       * @param squaredNorm quadratic model \f$q_2\f$ of a norm (generated with makeQuadraticNormModel(...))
       * @param omega estimate of the Lipschitz constant of the second derivative of the Lagrangian
       */
      CubicModel(const Quadratic& quadraticModel, const Quadratic& squaredNorm, double omega);

      /**
       * @brief Evaluate cubic model \f$ q(t) = q_1(t) + \frac{\omega}{6}q_2^{3/2} \f$.
       * @param t argument
       * @return \f$ q(t) = q_1(t) + \frac{\omega}{6}q_2^{3/2} \f$
       */
      Real operator()(Real t) const;

    private:
      Quadratic quadraticModel_;
      Quadratic squaredNorm_;
      double omega_;
    };

    /**
     * @brief Generate cubic regularized model for the affine covariant composite step method of \cite Lubkoll2015, \cite Lubkoll2015a.
     * @param nu normal step damping factor
     * @param dn normal step
     * @param dt tangential step
     * @param L Lagrange functional
     * @param x current iterate
     * @param omega estimate of the Lipschitz constant of the second derivative of the Lagrangian
     * @return CubicModel( makeQuadraticModel(nu,dn,dt,L,x), makeQuadraticNormModel(nu,dn,dt), omega )
     */
    CubicModel makeCubicModel(DampingFactor nu, const Vector& dn, const Vector& dt,
                              const C2Functional& L, const Vector& x, double omega);
  }

  /** @} */
}
