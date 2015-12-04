#ifndef SPACY_DYNAMIC_OPERATOR_HH
#define SPACY_DYNAMIC_OPERATOR_HH

#include <functional>
#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"
#include "Spacy/Util/smartPointer.hh"
#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/Mixins/target.hh"

#include "linearOperator.hh"
#include "vectorSpace.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * \anchor DynamicCallableOperatorAnchor
   * \brief Simplest possible time-dependent operator. Can store objects that satisfy the requirements of \ref DynamicCallableOperatorConceptAnchor "DynamicCallableOperatorConcept".
   *
   * Use this if only application of an operator is required.
   */
  using DynamicCallableOperator = std::function<Vector(double,const Vector&)>;


  /// Type-erased time-dependent operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicOperator : public Mixin::ToTarget<DynamicOperator>
  {
    friend class ToTarget<DynamicOperator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector& x) const = 0;
      virtual LinearOperator M() const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
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

      LinearOperator M() const final override
      {
        return this->get().M();
      }

      const VectorSpace& domain() const final override
      {
        return this->get().domain();
      }

      const VectorSpace& range() const final override
      {
        return this->get().range();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->get());
      }
    };

  public:
    DynamicOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,DynamicOperator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_M<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value> >
    DynamicOperator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,DynamicOperator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_M<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value> >
    DynamicOperator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(const Vector& x) const;

    LinearOperator M() const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };


  /// Type-erased time-dependent linear operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicLinearOperator : public Mixin::ToTarget<DynamicLinearOperator>
  {
    friend class ToTarget<DynamicLinearOperator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(double t, const Vector& x) const = 0;

  //    Vector operator()(const DynamicLinearOperator& x) const;

      virtual void add(const DynamicLinearOperator& y) = 0;
      virtual void subtract(const DynamicLinearOperator& y) = 0;
      virtual void multiply(double a) = 0;
      virtual DynamicLinearOperator negate() const = 0;
      virtual bool operator==(const DynamicLinearOperator& y) const = 0;
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

      Vector operator()(double t, const Vector& x) const final override
      {
        return this->get()(t,x);
      }

  //    Vector operator()(const DynamicLinearOperator& x) const;

      void add(const DynamicLinearOperator& y) final override
      {
        this->get() += (*y.template target<Impl>());
      }
      void subtract(const DynamicLinearOperator& y) final override
      {
        this->get() -= (*y.template target<Impl>());
      }

      void multiply(double a) final override
      {
        this->get() *= a;
      }

      DynamicLinearOperator negate() const final override
      {
        return DynamicLinearOperator( -this->get() );
      }

      bool operator==(const DynamicLinearOperator& y) const final override
      {
        return this->get() == (*y.template target<Impl>());
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
    DynamicLinearOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::DynamicLinearOperator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemOp_add<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_subtract<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_multiply<std::decay_t<Impl>>::value>,
//              class = std::enable_if_t<HasMemOp_negate<Impl>::value>,
              class = void_t< TryMemOp_negate<std::decay_t<Impl>> >,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_space<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_solver<std::decay_t<Impl>>::value> >
    DynamicLinearOperator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::DynamicLinearOperator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemOp_add<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_subtract<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_multiply<std::decay_t<Impl>>::value>,
              class = void_t< TryMemOp_negate<std::decay_t<Impl>> >,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_space<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_solver<std::decay_t<Impl>>::value> >
    DynamicLinearOperator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(double t, const Vector& x) const;

//    Vector operator()(const DynamicLinearOperator& x) const;

    DynamicLinearOperator& operator+=(const DynamicLinearOperator& y);

    DynamicLinearOperator& operator-=(const DynamicLinearOperator& y);

    DynamicLinearOperator& operator*=(double a);

    DynamicLinearOperator operator-() const;

    bool operator==(const DynamicLinearOperator& y) const;

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


  /// Type-erased time-dependent differentiable operator \f$A:\ [0,T] \times X \to Y \f$.
  class DynamicC1Operator : public Mixin::ToTarget<DynamicC1Operator>
  {
    friend class ToTarget<DynamicC1Operator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(double t, const Vector& x) const = 0;
      virtual Vector d1(double t, const Vector &x, const Vector &dx) const = 0;
      virtual LinearOperator linearization(double t, const Vector &x) const = 0;
      virtual LinearOperator M() const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base : AbstractBase, Mixin::Get<Impl>
    {
      Base(Impl const& impl)
        : Mixin::Get<Impl>(impl)
      {}

      Base(Impl&& impl)
        : Mixin::Get<Impl>(std::move(impl))
      {}

      Vector operator()(double t, const Vector& x) const final override
      {
        return this->get()(t,x);
      }

      Vector d1(double t, const Vector &x, const Vector &dx) const final override
      {
        return this->get().d1(t,x,dx);
      }

      LinearOperator linearization(double t, const Vector &x) const final override
      {
        return this->get().linearization(t,x);
      }

      LinearOperator M() const final override
      {
        return this->get().M();
      }

      const VectorSpace& domain() const final override
      {
        return this->get().domain();
      }

      const VectorSpace& range() const final override
      {
        return this->get().range();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->get());
      }
    };

  public:
    DynamicC1Operator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,DynamicC1Operator>::value>/*,
//              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_M<std::decay_t<Impl>>::value>,
              class = void_t< TryMemFn_d1_DynamicOperator<std::decay_t<Impl>,Vector> >,
              class = std::enable_if_t<HasMemFn_linearization<std::decay_t<Impl>,double,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value>*/ >
    DynamicC1Operator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,DynamicC1Operator>::value>/*,
//              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_M<std::decay_t<Impl>>::value>,
              class = void_t< TryMemFn_d1_DynamicOperator<std::decay_t<Impl>,Vector> >,
              class = std::enable_if_t<HasMemFn_linearization<std::decay_t<Impl>,double,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value>*/ >
    DynamicC1Operator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(double t, const Vector& x) const;

    /// Compute directional derivative \f$A'(x)\delta x\f$.
    Vector d1(double t, const Vector &x, const Vector &dx) const;

    /// Get linearization \f$A'(x):\ X\to Y \f$
    LinearOperator linearization(double t, const Vector& x) const;

    LinearOperator M() const;

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

#endif // SPACY_DYNAMIC_OPERATOR_HH

