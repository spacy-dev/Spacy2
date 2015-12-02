// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_C2_FUNCTIONAL_HH
#define SPACY_C2_FUNCTIONAL_HH

#include <functional>
#include "Spacy/Util/Concepts/c2FunctionalConcept.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Mixins/target.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/linearOperator.hh"

namespace Spacy
{
  /// @cond
  class LinearOperator;
  class Real;
  class Vector;
  class VectorSpace;
  /// @endcond

  /** @addtogroup SpacyGroup
   * @{
   */

  /// Type-erased twice differentiable functional \f$f:\ X \to \mathbb{R} \f$.
  class C2Functional : public Mixin::ToTarget<C2Functional>
  {
    friend class ToTarget<C2Functional>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Real operator()(const Vector& x) const = 0;
      virtual Vector d1(const Vector &x) const = 0;
      virtual Vector d2(const Vector &x, const Vector &dx) const = 0;
      virtual LinearOperator hessian(const Vector &x) const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base : AbstractBase, Mixin::Impl<Impl>
    {
      Base(Impl const& impl)
        : Mixin::Impl<Impl>(impl)
      {}

      Base(Impl&& impl)
        : Mixin::Impl<Impl>(std::move(impl))
      {}

      Real operator()(const Vector& x) const final override
      {
        return this->impl()(x);
      }

      Vector d1(const Vector &x) const final override
      {
        return this->impl().d1(x);
      }

      Vector d2(const Vector &x, const Vector &dx) const final override
      {
        return this->impl().d2(x,dx);
      }

      LinearOperator hessian(const Vector &x) const final override
      {
        return this->impl().hessian(x);
      }

      const VectorSpace& domain() const final override
      {
        return this->impl().domain();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->impl());
      }
    };

  public:
    C2Functional() = default;

    /// Construct from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C2Functional>::value>,
              class = std::enable_if_t<C2FunctionalConcept<std::decay_t<Impl>>::value> >
    C2Functional(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C2Functional>::value>,
              class = std::enable_if_t<C2FunctionalConcept<std::decay_t<Impl>>::value> >
    C2Functional& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }

    /// Apply operator.
    Real operator()(const Vector& x) const;

    /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
    Vector d1(const Vector &x) const;

    /// Compute second derivative as function space element in \f$X^*\f$, where \f$x,dx\in X\f$.
    Vector d2(const Vector &x, const Vector& dx) const;

    /// Access hessian as linear operator \f$ X \rightarrow X^* \f$.
    LinearOperator hessian(const Vector &x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };


  /**
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f'\f$ at \f$x\in X\f$ as dual element \f$ f'(x) \in X^* \f$.
   *
   * Equivalent to calling f.d1(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f'(x)\f$, i.e. f.d1(x).
   * @see C2Functional
   */
  Vector d1(const C2Functional& f, const Vector& x);

  /**
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f''\f$ at \f$x\in X\f$ as linear opeator \f$ f''(x): X \to X^* \f$.
   *
   * Equivalent to calling f.hessian(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f''(x)\f$, i.e. f.hessian(x).
   * @see C2Functional
   */
  LinearOperator d2(const C2Functional& f, const Vector& x);
  /** @} */
}

#endif // SPACY_C2_FUNCTIONAL_HH
