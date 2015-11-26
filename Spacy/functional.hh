#ifndef SPACY_FUNCTIONAL_HH
#define SPACY_FUNCTIONAL_HH

#include <boost/type_erasure/any.hpp>
#include "Util/Concepts/functionalConcept.hh"
#include "Util/Mixins/impl.hh"
#include "Util/Mixins/target.hh"

#include "Spacy/vectorSpace.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @anchor FunctionalAnchor
   * @brief Functional. Can store objects that satisfy the requirements of \ref FunctionalConceptAnchor "FunctionalConcept".
   */
//  using Functional = boost::type_erasure::any<Concepts::FunctionalConcept>;
  class Functional : public Mixin::Target<Functional>
  {
    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Real operator()(const Vector&) const = 0;
      virtual const VectorSpace& domain() const = 0;
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

      Real operator()(const Vector& x) const final override
      {
        return this->impl()(x);
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
    Functional() = default;

    template <class Impl>
    Functional(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl>
    Functional& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    Real operator()(const Vector& x) const;

    const VectorSpace& domain() const;

    operator bool() const;

  private:
    Mixin::CopyViaCloneUniqueImpl<AbstractBase> base_ = {};
  };

  /**
   * @ingroup SpacyGroup
   * @anchor C1FunctionalAnchor
   * @brief Differentiable functional. Can store objects that satisfy the requirements of \ref C1FunctionalConceptAnchor "C1FunctionalConcept".
   */
//  using C1Functional = boost::type_erasure::any<Concepts::C1FunctionalConcept>;

  class C1Functional : public Mixin::Target<Functional>
  {
    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Real operator()(const Vector&) const = 0;
      virtual Vector d1(const Vector&) const = 0;
      virtual const VectorSpace& domain() const = 0;
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

      Real operator()(const Vector& x) const final override
      {
        return this->impl()(x);
      }

      Vector d1(const Vector& x) const final override
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

    template <class Impl>
    C1Functional(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl>
    C1Functional& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    Real operator()(const Vector& x) const;

    Vector d1(const Vector& x) const;

    const VectorSpace& domain() const;

    operator bool() const;

  private:
    Mixin::CopyViaCloneUniqueImpl<AbstractBase> base_ = {};
  };

  /**
   * @ingroup SpacyGroup
   * @anchor C2FunctionalAnchor
   * @brief Twice differentiable functional. Can store objects that satisfy the requirements of \ref C2FunctionalConceptAnchor "C2FunctionalConcept".
   */
  using C2Functional = boost::type_erasure::any<Concepts::C2FunctionalConcept>;


  /**
   * @ingroup SpacyGroup
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f'\f$ at \f$x\in X\f$ as dual element \f$ f'(x) \in X^* \f$.
   *
   * This function is provided for providing unified access to the k-th derivative via dk(f,x)(arg0)...(argn).
   * Equivalent to calling f.d1(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f'(x)\f$, i.e. f.d1(x).
   * @see @ref C2FunctionalAnchor "C2Functional"
   */
  Vector d1(const C2Functional& f, const Vector& x);
//  boost::type_erasure::any< Concepts::VectorConcept > d1(const C2Functional& f, const boost::type_erasure::any< Concepts::VectorConcept >& x);

  /**
   * @ingroup SpacyGroup
   * @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f''\f$ at \f$x\in X\f$ as linear opeator \f$ f''(x): X \to X^* \f$.
   *
   * This function is provided for providing unified access to the k-th derivative via dk(f,x)(arg0)...(argn).
   * Equivalent to calling f.hessian(x).
   *
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f''(x)\f$, i.e. f.hessian(x).
   * @see @ref C2FunctionalAnchor "C2Functional"
   */
  boost::type_erasure::any< Concepts::LinearOperatorConcept > d2(const C2Functional& f, const Vector& x);
//  boost::type_erasure::any< Concepts::LinearOperatorConcept > d2(const C2Functional& f, const boost::type_erasure::any< Concepts::VectorConcept >& x);
}

#endif // SPACY_FUNCTIONAL_HH
