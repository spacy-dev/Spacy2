// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_FUNCTIONAL_HH
#define SPACY_FUNCTIONAL_HH

#include <functional>
#include "Spacy/Util/typeErasedStorage.hh"
#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"

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

  /// Type-erased functional.
  class Functional : public TypeErasedStorage
  {
  public:
    Functional() = default;

    /// Construct from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    Functional(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl))
    {
      init<Impl>();
    }

    /// Assign from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    Functional& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
      return *this;
    }

    /// Apply functional.
    Real operator()(const Vector& x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    template <class Impl>
    void init()
    {
      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
    }

    std::function<Real(const Vector&)> apply_;
    std::function<const VectorSpace&()> domain_;
  };


  /// Type-erased differentiable functional.
  class C1Functional : public TypeErasedStorage
  {
  public:
    C1Functional() = default;

    /// Construct from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_d1_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    C1Functional(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl))
    {
      init<Impl>();
    }

    /// Assign from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_d1_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    C1Functional& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
      return *this;
    }

    /// Apply functional.
    Real operator()(const Vector& x) const;

    /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
    Vector d1(const Vector &x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    template <class Impl>
    void init()
    {
      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
      d1_ = std::bind( &std::decay_t<Impl>::d1, Spacy::target<Impl>(*this) , std::placeholders::_1 );
      domain_ = std::bind( &std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
    }

    std::function<Real(const Vector&)> apply_;
    std::function<Vector(const Vector&)> d1_;
    std::function<const VectorSpace&()> domain_;
  };


  /// Type-erased twice differentiable functional.
  class C2Functional : public TypeErasedStorage
  {
  public:
    C2Functional() = default;

    /// Construct from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C2Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_d1_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d2_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_hessian<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    C2Functional(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl))
    {
      init<Impl>();
    }

    /// Assign from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C2Functional>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemFn_d1_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d2_Functional<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_hessian<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value> >
    C2Functional& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
      return *this;
    }

    /// Apply functional.
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
    template <class Impl>
    void init()
    {
      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
      d1_ = std::bind(&std::decay_t<Impl>::d1, Spacy::target<Impl>(*this) , std::placeholders::_1 );
      d2_ = std::bind(&std::decay_t<Impl>::d2, Spacy::target<Impl>(*this) , std::placeholders::_1 , std::placeholders::_2 );
      hessian_ = std::bind(&std::decay_t<Impl>::hessian, Spacy::target<Impl>(*this) , std::placeholders::_1 );
      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
    }

    std::function<Real(const Vector&)> apply_;
    std::function<Vector(const Vector&)> d1_;
    std::function<Vector(const Vector&,const Vector&)> d2_;
    std::function<LinearOperator(const Vector &x)> hessian_;
    std::function<const VectorSpace&()> domain_;
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
//  boost::type_erasure::any< Concepts::LinearOperatorConcept > d2(const C2Functional& f, const boost::type_erasure::any< Concepts::VectorConcept >& x);
  /** @} */
}

#endif // SPACY_FUNCTIONAL_HH
