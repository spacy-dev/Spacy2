// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITHM_CG_TERMINATION_CRITERION_HH
#define SPACY_ALGORITHM_CG_TERMINATION_CRITERION_HH

#include "Spacy/Util/memFnChecks.hh"
#include "Spacy/Util/memOpChecks.hh"
#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/smartPointer.hh"
#include "Spacy/Util/Concepts/CG/terminationCriterionConcept.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace CG
  {
    /// Type-erased termination criterion for conjugate gradient methods.
    class TerminationCriterion
    {
      struct AbstractBase
      {
        virtual ~AbstractBase() = default;
        virtual bool operator()() const = 0;
        virtual void clear() = 0;
        virtual void update(double,double,double,double) = 0;
        virtual bool vanishingStep() const = 0;
        virtual bool minimalDecreaseAchieved() const = 0;
        virtual void setEps(double) = 0;
        virtual void setAbsoluteAccuracy(double) = 0;
        virtual void setMinimalAccuray(double) = 0;
        virtual void setRelativeAccuracy(double) = 0;
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

        bool operator()() const
        {
          return this->get()();
        }

        void clear()
        {
          this->get().clear();
        }

        void update(double alpha, double qAq, double qPq, double rPINVr)
        {
          return this->get().update(alpha,qAq,qPq,rPINVr);
        }

        bool vanishingStep() const
        {
          return this->get().vanishingStep();
        }

        bool minimalDecreaseAchieved() const
        {
          return this->get().minimalDecreaseAchieved();
        }

        void setEps(double eps)
        {
          this->get().setEps(eps);
        }

        void setAbsoluteAccuracy(double accuracy)
        {
          this->get().setAbsoluteAccuracy(accuracy);
        }

        void setMinimalAccuray(double accuracy)
        {
          this->get().setMinimalAccuracy(accuracy);
        }

        void setRelativeAccuracy(double accuracy)
        {
          this->get().setRelativeAccuracy(accuracy);
        }

        std::unique_ptr<AbstractBase> clone() const
        {
          return std::make_unique<Base>(this->get());
        }

      };

    public:
      TerminationCriterion() = default;

      /// Construct from implementation.
      template <class Impl,
                class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,TerminationCriterion>::value>,
                class = std::enable_if_t<TerminationCriterionConcept<std::decay_t<Impl>>::value> >
      TerminationCriterion(Impl&& impl)
        : base_( std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl)) )
      {}

      /// Assign from implementation.
      template <class Impl,
                class = std::enable_if_t<!std::is_same<std::decay_t<Impl>,TerminationCriterion>::value>,
                class = std::enable_if_t<TerminationCriterionConcept<std::decay_t<Impl>>::value> >
      TerminationCriterion& operator=(Impl&& impl)
      {
        base_ = std::make_unique< Base< std::decay_t<Impl> > >(std::forward<Impl>(impl));
        return *this;
      }

      bool operator()() const;

      void clear();

      void update(Real alpha, Real qAq, Real qPq, Real rPINVr);

      bool vanishingStep() const;

      bool minimalDecreaseAchieved() const;

      void setEps(double eps);

      void setAbsoluteAccuracy(double accuracy);

      void setMinimalAccuray(double accuracy);

      void setRelativeAccuracy(double accuracy);

      /// Check if an implementation has been assigned.
      operator bool() const;

    private:
      CopyViaClonePtr<AbstractBase> base_;
    };
  }
}

#endif // SPACY_ALGORITHM_CG_TERMINATION_CRITERION_HH
