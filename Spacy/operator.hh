#ifndef SPACY_OPERATOR_HH
#define SPACY_OPERATOR_HH

#include <boost/type_erasure/any.hpp>

#include "Util/Concepts/operatorConcept.hh"
#include "Util/Concepts/vectorConcept.hh"

#include "Util/Mixins/impl.hh"
#include "Util/Mixins/target.hh"

#include "vectorSpace.hh"

namespace Spacy
{
  /**
   * \ingroup SpacyGroup
   * \anchor CallableOperatorAnchor
   * \brief Simplest possible operator class. Can store objects that satisfy the requirements of \ref CallableOperatorConceptAnchor "CallableOperatorConcept".
   *
   * Use this if only application of an operator is required.
   */
  //using CallableOperator = boost::type_erasure::any< Concepts::CallableOperatorConcept >;
  using CallableOperator = std::function<Vector(const Vector&)>;

  /**
   * \ingroup SpacyGroup
   * \anchor LinearOperatorAnchor
   * \brief Linear operator class. Can store objects that satisfy the requirements of \ref LinearOperatorConceptAnchor "LinearOperatorConcept".
   */
  using LinearOperator = boost::type_erasure::any< Concepts::LinearOperatorConcept >;

//  class LinearOperator : public Mixin::Target<LinearOperator>
//  {
//    struct AbstractBase
//    {
//      virtual ~AbstractBase(){}
//      virtual Vector operator()(const Vector&) const = 0;
//      virtual std::function<Vector(const Vector&)> solver() const = 0;
//      virtual LinearOperator& operator+=(const LinearOperator& y) = 0;
//      virtual LinearOperator& operator-=(const LinearOperator& y) = 0;
//      virtual LinearOperator& operator*=(double a) = 0;
//      virtual LinearOperator operator-(const LinearOperator& y) const = 0;
//      virtual bool operator==(const LinearOperator& y) const = 0;
//      virtual Vector operator()(const LinearOperator& y) const = 0;
//      virtual const VectorSpace* space() const = 0;
//      virtual const VectorSpace& domain() const = 0;
//      virtual const VectorSpace& range() const = 0;
//      virtual std::unique_ptr<AbstractBase> clone() const = 0;
//    };

//    template <class Impl>
//    struct Base :
//        public AbstractBase,
//        public Mixin::CopyingUniqueImpl<Impl>
//    {
//      explicit Base(const Impl& impl)
//        : Mixin::CopyingUniqueImpl<Impl>(std::make_unique<Impl>(impl))
//      {}

//      Vector operator()(const Vector& x) const final override
//      {
//        return this->impl()(x);
//      }

//      std::function<Vector(const Vector&)> solver() const
//      {
//        return this->impl().solver();
//      }

//      LinearOperator& operator+=(const LinearOperator& y)
//      {
//        return this->impl() += *y.template target<Impl>();
//      }

//      LinearOperator& operator-=(const LinearOperator& y)
//      {
//        return this->impl() -= *y.template target<Impl>();
//      }

//      LinearOperator& operator*=(double a)
//      {
//        return this->impl() *= a;
//      }

//      LinearOperator operator-(const LinearOperator& y) const
//      {
//        return - (*y.template target<Impl>());
//      }

//      bool operator==(const LinearOperator& y) const
//      {
//        return this->impl() == *y.template target<Impl>();
//      }

//      Vector operator()(const LinearOperator& y) const
//      {
//        return this->impl()( *y.template target<Impl>() );
//      }

//      const VectorSpace* space() const
//      {
//        return this->impl().space();
//      }

//      const VectorSpace& domain() const final override
//      {
//        return this->impl().domain();
//      }

//      const VectorSpace& range() const final override
//      {
//        return this->impl().range();
//      }

//      std::unique_ptr<AbstractBase> clone() const final override
//      {
//        return std::make_unique< Base<Impl> >(this->impl());
//      }
//    };

//  public:
//    LinearOperator() = default;

//    template <class Impl>
//    LinearOperator(Impl&& impl)
//      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
//    {}

//    template <class Impl>
//    LinearOperator& operator=(Impl&& impl)
//    {
//      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
//      return *this;
//    }

//    Vector operator()(const Vector& x) const;

//    const VectorSpace& domain() const;

//    const VectorSpace& range() const;

//    operator bool() const;

//  private:
//    Mixin::CopyViaCloneUniqueImpl<AbstractBase> base_ = {};
//  };

  /**
   * \ingroup SpacyGroup
   * \anchor OperatorAnchor
   * \brief Operator class. Can store objects that satisfy the requirements of \ref OperatorConceptAnchor "OperatorConcept".
   */
//  using Operator = boost::type_erasure::any< Concepts::OperatorConcept >;

  class Operator : public Mixin::Target<Operator>
  {
    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector&) const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base :
        public AbstractBase,
        public Mixin::CopyingUniqueImpl<Impl>
    {
      explicit Base(const Impl& impl)
        : Mixin::CopyingUniqueImpl<Impl>(std::make_unique<Impl>(impl))
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

    template <class Impl>
    Operator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl>
    Operator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    Vector operator()(const Vector& x) const;

    const VectorSpace& domain() const;

    const VectorSpace& range() const;

    operator bool() const;

  private:
    Mixin::CopyViaCloneUniqueImpl<AbstractBase> base_ = {};
  };

  class C1Operator : public Mixin::Target<C1Operator>
  {
    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector&) const = 0;
      virtual Vector d1(const Vector&, const Vector&) const = 0;
      virtual LinearOperator linearization(const Vector&) const = 0;
      virtual const VectorSpace& domain() const = 0;
      virtual const VectorSpace& range() const = 0;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base :
        public AbstractBase,
        public Mixin::CopyingUniqueImpl<Impl>
    {
      explicit Base(const Impl& impl)
        : Mixin::CopyingUniqueImpl<Impl>(std::make_unique<Impl>(impl))
      {}

      Vector operator()(const Vector& x) const final override
      {
        return this->impl()(x);
      }

      Vector d1(const Vector& x, const Vector& dx) const final override
      {
        return this->impl().d1(x,dx);
      }

      LinearOperator linearization(const Vector& x) const final override
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

    template <class Impl>
    C1Operator(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl>
    C1Operator& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    Vector operator()(const Vector& x) const;

    Vector d1(const Vector& x, const Vector& dx) const;

    LinearOperator linearization(const Vector& x) const;

    const VectorSpace& domain() const;

    const VectorSpace& range() const;

    operator bool() const;

  private:
    Mixin::CopyViaCloneUniqueImpl<AbstractBase> base_ = {};
  };

  /**
   * \ingroup SpacyGroup
   * \anchor C1OperatorAnchor
   * \brief Differentiable operator class. Can store objects that satisfy the requirements of \ref C1OperatorConceptAnchor "C1OperatorConcept".
   */
//  using C1Operator = boost::type_erasure::any< Concepts::C1OperatorConcept >;

  /**
   * \ingroup SpacyGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  std::function<Vector(const Vector&)> operator^(const LinearOperator& A, int k);

  /**
   * \ingroup SpacyGroup
   * \brief Access solver via A^-1. Throws for k!=-1.
   */
  std::function<Vector(const Vector&)> operator^(LinearOperator&& A, int k);

  /**
   * @ingroup SpacyGroup
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
}

#endif // SPACY_OPERATOR_HH
