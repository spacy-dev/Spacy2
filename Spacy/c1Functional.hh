// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_C1_FUNCTIONAL_HH
#define SPACY_C1_FUNCTIONAL_HH

#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/Concepts/c1FunctionalConcept.hh"
#include "Spacy/Util/smartPointer.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Mixins/target.hh"

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  class VectorSpace;
  /// @endcond

  /** @addtogroup SpacyGroup
   * @{
   */

  /// Type-erased differentiable functional \f$f:\ X \to \mathbb{R} \f$.
  class C1Functional : public Mixin::ToTarget<C1Functional>
  {
    friend class ToTarget<C1Functional>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Real operator()(const Vector& x) const = 0;
      virtual Vector d1(const Vector &x) const = 0;
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
    C1Functional() = default;

    /// Construct from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Functional>::value>,
              class = std::enable_if_t<C1FunctionalConcept<std::decay_t<Impl>>::value> >
    C1Functional(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from functional implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Functional>::value>,
              class = std::enable_if_t<C1FunctionalConcept<std::decay_t<Impl>>::value> >
    C1Functional& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Real operator()(const Vector& x) const;

    /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
    Vector d1(const Vector &x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };

  /** @} */
}

#endif // SPACY_C1_FUNCTIONAL_HH
