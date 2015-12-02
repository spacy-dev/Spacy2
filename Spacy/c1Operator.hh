// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_C1_OPERATOR_HH
#define SPACY_C1_OPERATOR_HH

#include "Spacy/Util/Concepts/c1OperatorConcept.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Mixins/target.hh"
#include "Spacy/Util/smartPointer.hh"
#include "Spacy/linearOperator.hh"

/** @addtogroup SpacyGroup
 * @{
 */
namespace Spacy
{
  /// Type-erased differentiable operator \f$A:\ X \to Y \f$.
  class C1Operator : public Mixin::ToTarget<C1Operator>
  {
    friend class ToTarget<C1Operator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector& x) const = 0;
      virtual Vector d1(const Vector &x, const Vector &dx) const = 0;
      virtual LinearOperator linearization(const Vector &x) const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
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

      Vector operator()(const Vector& x) const final override
      {
        return this->impl()(x);
      }

      Vector d1(const Vector &x, const Vector &dx) const final override
      {
        return this->impl().d1(x,dx);
      }

      LinearOperator linearization(const Vector &x) const final override
      {
        return this->impl().linearization(x);
      }

      const VectorSpace& domain() const final override
      {
        return this->impl().domain();
      }

      const VectorSpace& range() const final override
      {
        return this->impl().range();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->impl());
      }
    };

  public:
    C1Operator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Operator>::value>,
              class = std::enable_if_t<C1OperatorConcept<std::decay_t<Impl>>::value> >
    C1Operator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Operator>::value>,
              class = std::enable_if_t<C1OperatorConcept<std::decay_t<Impl>>::value> >
    C1Operator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(const Vector& x) const;

    /// Compute directional derivative \f$A'(x)\delta x\f$.
    Vector d1(const Vector &x, const Vector &dx) const;

    /// Get linearization \f$A'(x):\ X\to Y \f$
    LinearOperator linearization(const Vector& x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };
}
/** @} */

#endif // SPACY_C1_OPERATOR_HH
