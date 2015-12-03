// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_LINEAR_OPERATOR_HH
#define SPACY_LINEAR_OPERATOR_HH

#include "Spacy/Util/Concepts/linearOperatorConcept.hh"
#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/Mixins/target.hh"
#include "Spacy/Util/smartPointer.hh"
#include "Spacy/linearOperator.hh"
#include "Spacy/vector.hh"

/** @addtogroup SpacyGroup
 * @{
 */
namespace Spacy
{
  /// @cond
  class C1Operator;
  /// @endcond

  /// Type-erased linear operator \f$A:\ X \to Y \f$.
  class LinearOperator : public Mixin::ToTarget<LinearOperator>
  {
    friend class ToTarget<LinearOperator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector& x) const = 0;

  //    Vector operator()(const LinearOperator& x) const;

      virtual void add(const LinearOperator& y) = 0;
      virtual void subtract(const LinearOperator& y) = 0;
      virtual void multiply(double a) = 0;
      virtual LinearOperator negate() const = 0;
      virtual bool operator==(const LinearOperator& y) const = 0;
      virtual std::function<Vector(const Vector&)> solver() const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
      virtual const VectorSpace& space() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl, class = std::enable_if_t<!std::is_reference<Impl>::value> >
    struct Base : AbstractBase, Mixin::Get<Impl>
    {
      Base(Impl const& impl)
        : Mixin::Get<Impl>(impl)
      {}

      Base(Impl&& impl)
        : Mixin::Get<Impl>(std::move(impl))
      {}

      Vector operator()(const Vector& x) const final override
      {
        return this->get()(x);
      }

  //    Vector operator()(const LinearOperator& x) const;

      void add(const LinearOperator& y) final override
      {
        this->get() += (*::Spacy::target<Impl>(y));
      }
      void subtract(const LinearOperator& y) final override
      {
        this->get() -= (*::Spacy::target<Impl>(y));
      }

      void multiply(double a) final override
      {
        this->get() *= a;
      }

      LinearOperator negate() const final override
      {
        return LinearOperator( -this->get() );
      }

      bool operator==(const LinearOperator& y) const final override
      {
        return this->get() == (*::Spacy::target<Impl>(y));
      }

      std::function<Vector(const Vector&)> solver() const final override
      {
        return this->get().solver();
      }

      const VectorSpace& domain() const final override
      {
        return this->get().domain();
      }

      const VectorSpace& range() const final override
      {
        return this->get().range();
      }

      const VectorSpace& space() const final override
      {
        return this->get().space();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->get());
      }
    };

  public:
    LinearOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<LinearOperatorConcept<std::decay_t<Impl>>::value> >
    LinearOperator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<LinearOperatorConcept<std::decay_t<Impl>>::value> >
    LinearOperator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(const Vector& x) const;

//    Vector operator()(const LinearOperator& x) const;

    LinearOperator& operator+=(const LinearOperator& y);

    LinearOperator& operator-=(const LinearOperator& y);

    LinearOperator& operator*=(double a);

    LinearOperator operator-() const;

    bool operator==(const LinearOperator& y) const;

    std::function<Vector(const Vector&)> solver() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Access underlying space of linear operators.
    const VectorSpace& space() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };

  /// Access solver via A^-1. Throws for k!=-1.
  std::function<Vector(const Vector&)> operator^(const LinearOperator& A, int k);

  /// Access solver via A^-1. Throws for k!=-1.
  std::function<Vector(const Vector&)> operator^(LinearOperator&& A, int k);

  /**
   * @brief For an operator \f$ A: X\to Y \f$, compute \f$A'\f$ at \f$x\in X\f$ as linear operator \f$ A'(x): X \to Y \f$.
   *
   * Equivalent to calling A.linearization(x).
   *
   * @param A differentiable operator
   * @param x point of linearization
   * @return \f$A'(x)\f$, i.e. A.linearization(x).
   * @see @ref C1OperatorAnchor "C1Operator", @ref LinearOperatorAnchor "LinearOperator"
   */
  LinearOperator d1(const C1Operator& A, const Vector& x);

  LinearOperator& axpy(LinearOperator& A, double a, LinearOperator B);
  /** @} */
}

#endif // SPACY_LINEAR_OPERATOR_HH
