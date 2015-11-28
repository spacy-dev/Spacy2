#ifndef SPACY_OPERATOR_HH
#define SPACY_OPERATOR_HH

#include "Util/memFnChecks.hh"
#include "Util/memOpChecks.hh"
#include "Util/Mixins/impl.hh"
#include "Util/Mixins/target.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  class VectorSpace;
  /// @endcond

  /** @addtogroup SpacyGroup
   * @{
   */

  /// Simplest possible operator definition.
  /// Type-erased operator \f$A:\ X \to Y \f$.
  class CallableOperator : public Mixin::ToTarget<CallableOperator>
  {
    friend class ToTarget<CallableOperator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector& x) const = 0;
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

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->impl());
      }
    };

  public:
    CallableOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,CallableOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value> >
    CallableOperator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,CallableOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value> >
    CallableOperator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(const Vector& x) const
    {
      return base_->operator ()(x);
    }

    /// Check if an implementation has been assigned.
    operator bool() const
    {
      return base_;
    }

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };


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
      virtual const VectorSpace* space() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl, class = std::enable_if_t<!std::is_reference<Impl>::value> >
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

  //    Vector operator()(const LinearOperator& x) const;

      void add(const LinearOperator& y) final override
      {
        this->impl() += (*y.template target<Impl>());
      }
      void subtract(const LinearOperator& y) final override
      {
        this->impl() -= (*y.template target<Impl>());
      }

      void multiply(double a) final override
      {
        this->impl() *= a;
      }

      LinearOperator negate() const final override
      {
        return LinearOperator( -this->impl() );
      }

      bool operator==(const LinearOperator& y) const final override
      {
        return this->impl() == (*y.template target<Impl>());
      }

      std::function<Vector(const Vector&)> solver() const final override
      {
        return this->impl().solver();
      }

      const VectorSpace& domain() const final override
      {
        return this->impl().domain();
      }

      const VectorSpace& range() const final override
      {
        return this->impl().range();
      }

      const VectorSpace* space() const final override
      {
        return this->impl().space();
      }

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(this->impl());
      }
    };

  public:
    LinearOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
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
    LinearOperator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemOp_add<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_subtract<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemOp_multiply<std::decay_t<Impl>>::value>,
              class = void_t< TryMemOp_negate<std::decay_t<Impl>> >,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_space<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_solver<std::decay_t<Impl>>::value> >
    LinearOperator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >(std::forward<Impl>(impl));
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
    const VectorSpace* space() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };


  /// Type-erased operator \f$A:\ X \to Y \f$.
  class Operator : public Mixin::ToTarget<Operator>
  {
    friend class ToTarget<Operator>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector& x) const = 0;
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
    Operator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    Operator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    Operator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >(std::forward<Impl>(impl));
      return *this;
    }


    /// Apply operator.
    Vector operator()(const Vector& x) const;

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

    /// Access range space \f$Y\f$.
    const VectorSpace& range() const;

    /// Check if an implementation has been assigned.
    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_;
  };


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
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d1_Operator<std::decay_t<Impl>,Vector>::value>,
              class = std::enable_if_t<HasMemFn_linearization<std::decay_t<Impl>,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value> >
    C1Operator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >(std::forward<Impl>(impl)) )
    {}

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,C1Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d1_Operator<std::decay_t<Impl>,Vector>::value>,
              class = std::enable_if_t<HasMemFn_linearization<std::decay_t<Impl>,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<std::decay_t<Impl>>::value>,
              class = std::enable_if_t<HasMemFn_range<std::decay_t<Impl>>::value> >
    C1Operator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >(std::forward<Impl>(impl));
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


//  /// Type-erased differentiable operator.
//  class C1Operator : public TypeErasedStorage
//  {
//  public:
//    C1Operator() = default;

//    /// Construct from operator implementation.
//    template <class Impl,
//              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::C1Operator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_d1_Operator<Impl,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_linearization<Impl,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
//              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
//    C1Operator(Impl&& impl)
//      : TypeErasedStorage(std::forward<Impl>(impl)),
//        apply_( std::cref(*Spacy::target<Impl>(*this) ) )
//    {
//      init<Impl>();
//    }

//    /// Assign from operator implementation.
//    template <class Impl,
//              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::C1Operator>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_d1_Operator<Impl,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_linearization<Impl,Vector>::value>,
//              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
//              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
//    C1Operator& operator=(Impl&& impl)
//    {
//      TypeErasedStorage::operator =(std::forward<Impl>(impl));
//      init<Impl>();
//      return *this;
//    }

//    /// Apply operator.
//    Vector operator()(const Vector& x) const;

//    /// Compute directional derivative \f$A'(x)\delta x\f$.
//    Vector d1(const Vector &x, const Vector &dx) const;

//    /// Get linearization \f$A'(x):\ X\to Y \f$
//    LinearOperator linearization(const Vector& x) const;

//    /// Access domain space \f$X\f$.
//    const VectorSpace& domain() const;

//    /// Access range space \f$Y\f$.
//    const VectorSpace& range() const;

//    /// Check if an implementation has been assigned.
//    operator bool() const;

//  private:
//    template <class Impl>
//    void init()
//    {
//      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
//      d1_ = std::bind(&std::decay_t<Impl>::d1, Spacy::target<Impl>(*this), std::placeholders::_1, std::placeholders::_2);
//      linearization_ = std::bind(&std::decay_t<Impl>::linearization, Spacy::target<Impl>(*this), std::placeholders::_1);
//      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
//      range_ = std::bind(&std::decay_t<Impl>::range, Spacy::target<Impl>(*this) );
//    }

//    std::function<Vector(const Vector&)> apply_;
//    std::function<Vector(const Vector&, const Vector&)> d1_;
//    std::function<LinearOperator(const Vector&)> linearization_;
//    std::function<const VectorSpace&()> domain_, range_;
//  };


  /**
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  std::function<Vector(const Vector&)> operator^(const LinearOperator& A, int k);

  /**
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
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
//  LinearOperator d1(const C1Operator& A, const boost::type_erasure::any< Concepts::VectorConcept >& x);

  /** @} */
}

#endif // SPACY_OPERATOR_HH
