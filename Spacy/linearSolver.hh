// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_LINEAR_SOLVER_HH
#define SPACY_LINEAR_SOLVER_HH

#include <functional>

#include "Util/Mixins/get.hh"
#include "Util/Mixins/target.hh"

#include "vector.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /// Type-erased linear solver.
  using LinearSolver = std::function<Vector(const Vector&)>;

  /// Type-erased indefinite linear solver. Additionally monitors if the underlying operator is positive definite.
  class IndefiniteLinearSolver : public Mixin::ToTarget<IndefiniteLinearSolver>
  {
    friend class Mixin::ToTarget<IndefiniteLinearSolver>;

    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual Vector operator()(const Vector&) const = 0;
      virtual bool isPositiveDefinite() const = 0 ;
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base :
        public AbstractBase,
        public Mixin::Get<Impl>
    {
      explicit Base(const Impl& impl)
        : Mixin::Get<Impl>(impl)
      {}

      explicit Base(Impl&& impl)
        : Mixin::Get<Impl>(std::move(impl))
      {}

      Vector operator()(const Vector& x) const final override
      {
        return this->get()(x);
      }

      bool isPositiveDefinite() const final override
      {
        return this->get().isPositiveDefinite();
      }

      std::unique_ptr<AbstractBase> clone() const
      {
        return std::make_unique< Base<Impl> >(this->get());
      }
    };

  public:
    IndefiniteLinearSolver() = default;

    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,IndefiniteLinearSolver>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t< HasMemFn_isPositiveDefinite<std::decay_t<Impl> >::value > >
    IndefiniteLinearSolver(Impl&& impl)
      : base_( std::make_unique< Base< std::decay_t<Impl> > >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl,
              class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,IndefiniteLinearSolver>::value>,
              class = std::enable_if_t<HasMemOp_callable<std::decay_t<Impl>,Vector,Vector>::value>,
              class = std::enable_if_t< HasMemFn_isPositiveDefinite<std::decay_t<Impl> >::value > >
    IndefiniteLinearSolver& operator=(Impl&& impl)
    {
      base_ = std::make_unique< Base< std::decay_t<Impl> > >( std::forward<Impl>(impl) );
      return *this;
    }

    Vector operator()(const Vector& x) const;

    bool isPositiveDefinite() const;

    operator bool() const;

  private:
    CopyViaClonePtr<AbstractBase> base_ = {};
  };
}

#endif // SPACY_LINEAR_SOLVER_HH
