// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_OPERATOR_HH
#define SPACY_OPERATOR_HH

#include "Spacy/Util/memOpChecks.hh"
#include "Spacy/Util/Concepts/operatorConcept.hh"
#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/Mixins/target.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  class VectorSpace;
  /// @endcond
  
  using CallableOperator = std::function<Vector(const Vector&)>;
  
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
    Operator() = default;
    
    /// Construct from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Operator>::value>,
              class = std::enable_if_t<OperatorConcept<std::decay_t<Impl>>::value> >
    Operator(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
    {}
    
    /// Assign from operator implementation.
    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,Operator>::value>,
              class = std::enable_if_t<OperatorConcept<std::decay_t<Impl>>::value> >
    Operator& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
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
}

#endif // SPACY_OPERATOR_HH
