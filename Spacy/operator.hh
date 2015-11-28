#ifndef SPACY_OPERATOR_HH
#define SPACY_OPERATOR_HH

#include "Util/typeErasedStorage.hh"
#include "Util/memFnChecks.hh"
#include "Util/memOpChecks.hh"

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
  using CallableOperator = std::function<Vector(const Vector&)>;


  /// Type-erased linear operator \f$A:\ X \to Y \f$.
  class LinearOperator : public TypeErasedStorage
  {
  public:
    LinearOperator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemOp_add<Impl>::value>,
              class = std::enable_if_t<HasMemOp_subtract<Impl>::value>,
              class = std::enable_if_t<HasMemOp_multiply<Impl>::value>,
              class = void_t< TryMemOp_negate<Impl> >,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value>,
              class = std::enable_if_t<HasMemFn_space<Impl>::value>,
              class = std::enable_if_t<HasMemFn_solver<Impl>::value> >
    LinearOperator(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl)),
        apply_( std::cref(*Spacy::target<Impl>(*this) ) )
    {
      init<Impl>();
    }

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::LinearOperator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
//              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Real>::value>,
              class = std::enable_if_t<HasMemOp_add<Impl>::value>,
              class = std::enable_if_t<HasMemOp_subtract<Impl>::value>,
              class = std::enable_if_t<HasMemOp_multiply<Impl>::value>,
              class = void_t< TryMemOp_negate<Impl> >,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value>,
              class = std::enable_if_t<HasMemFn_space<Impl>::value>,
              class = std::enable_if_t<HasMemFn_solver<Impl>::value> >
    LinearOperator& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
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
    template <class Impl>
    void init()
    {
      apply_ = [this](const Vector& x) { return (*Spacy::target<Impl>(*this))(x); };
//      applyAsDual_ = [this](const LinearOperator& y) { return (*Spacy::target<Impl>(*this))(y); };
      add_ = [this](const LinearOperator& y) -> LinearOperator& { (*Spacy::target<Impl>(*this)) += (*Spacy::target<Impl>(y)); return *this; };
      subtract_ = [this](const LinearOperator& y) -> LinearOperator& { (*Spacy::target<Impl>(*this)) -= (*Spacy::target<Impl>(y)); return *this; };
      multiply_ = [this](double a) -> LinearOperator& { (*Spacy::target<Impl>(*this)) *= a; return *this; };
      negate_ = [this]() -> LinearOperator { return LinearOperator( -(*Spacy::target<Impl>(*this)) ); };
      compare_ = [this](const LinearOperator& y) { return (*Spacy::target<Impl>(*this)) == (*Spacy::target<Impl>(y)); };
      solver_ = [this]() -> std::function<Vector(const Vector&)> { return Spacy::target<Impl>(*this)->solver(); };
      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
      range_ = std::bind(&std::decay_t<Impl>::range, Spacy::target<Impl>(*this) );
      space_ = std::bind(&std::decay_t<Impl>::space, Spacy::target<Impl>(*this));
    }

    std::function<Vector(const Vector&)> apply_;
//    std::function<Real(const Vector&)> applyAsDual_;
    std::function<LinearOperator&(const LinearOperator&)> add_, subtract_;
    std::function<LinearOperator&(double a)> multiply_;
    std::function<LinearOperator()> negate_;
    std::function<bool(const LinearOperator&)> compare_;
    std::function<std::function<Vector(const Vector&)>()> solver_;
    std::function<const VectorSpace&()> domain_, range_;
    std::function<const VectorSpace*()> space_;
  };


  /// Type-erased operator.
  class Operator : public TypeErasedStorage
  {
  public:
    Operator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    Operator(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl))
    {
      init<Impl>();
    }

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    Operator& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
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
    template <class Impl>
    void init()
    {
      apply_ = std::cref( *Spacy::target<Impl>(*this) );
//      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
      range_ = std::bind(&std::decay_t<Impl>::range, Spacy::target<Impl>(*this) );
    }

    std::function<Vector(const Vector&)> apply_;
    std::function<const VectorSpace&()> domain_, range_;
  };

  /// Type-erased differentiable operator.
  class C1Operator : public TypeErasedStorage
  {
  public:
    C1Operator() = default;

    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::C1Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d1_Operator<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_linearization<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    C1Operator(Impl&& impl)
      : TypeErasedStorage(std::forward<Impl>(impl)),
        apply_( std::cref(*Spacy::target<Impl>(*this) ) )
    {
      init<Impl>();
    }

    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,::Spacy::C1Operator>::value>,
              class = std::enable_if_t<HasMemOp_callable<Impl,Vector,Vector>::value>,
              class = std::enable_if_t<HasMemFn_d1_Operator<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_linearization<Impl,Vector>::value>,
              class = std::enable_if_t<HasMemFn_domain<Impl>::value>,
              class = std::enable_if_t<HasMemFn_range<Impl>::value> >
    C1Operator& operator=(Impl&& impl)
    {
      TypeErasedStorage::operator =(std::forward<Impl>(impl));
      init<Impl>();
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
    template <class Impl>
    void init()
    {
      apply_ = std::bind(&std::decay_t<Impl>::operator(), Spacy::target<Impl>(*this), std::placeholders::_1);
      d1_ = std::bind(&std::decay_t<Impl>::d1, Spacy::target<Impl>(*this), std::placeholders::_1, std::placeholders::_2);
      linearization_ = std::bind(&std::decay_t<Impl>::linearization, Spacy::target<Impl>(*this), std::placeholders::_1);
      domain_ = std::bind(&std::decay_t<Impl>::domain, Spacy::target<Impl>(*this) );
      range_ = std::bind(&std::decay_t<Impl>::range, Spacy::target<Impl>(*this) );
    }

    std::function<Vector(const Vector&)> apply_;
    std::function<Vector(const Vector&, const Vector&)> d1_;
    std::function<LinearOperator(const Vector&)> linearization_;
    std::function<const VectorSpace&()> domain_, range_;
  };


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
