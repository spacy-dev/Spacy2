#ifndef SPACY_ALGORITHM_GENERIC_ITERATIVE_METHOD_HH
#define SPACY_ALGORITHM_GENERIC_ITERATIVE_METHOD_HH

#include <functional>
#include <memory>
#include <ostream>
#include <utility>

#include "fglue/TMP/bind.hh"
#include "fglue/TMP/createMissingBaseClasses.hh"

#include "optional.hh"

#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Exceptions/notConvergedException.hh"

#define SPACY_MIXIN_LIST Mixin::RelativeAccuracy, Mixin::AbsoluteAccuracy, Mixin::MinimalAccuracy

#ifdef SPACY_MIXIN_LIST
  #define TMP_LIST SPACY_MIXIN_LIST
  #undef SPACY_MIXIN_LIST
  #define SPACY_MIXIN_LIST TMP_LIST, Mixin::Eps
#else
  #define SPACY_MIXIN_LIST Mixin::Eps
#endif

namespace Spacy
{
  /** @addtogroup GenericGroup @{ */
  namespace Generic
  {
    /// Generic module for iterative methods.
    template <class Step, class TerminationCriterion>
    class IterativeMethod :
        public Step ,
        public Mixin::MaxSteps ,
        public FGlue::Apply< FGlue::Compose,
          FGlue::Apply< FGlue::Variadic< FGlue::StoreIf<
              FGlue::Apply<FGlue::Delay<FGlue::And>,
                FGlue::IsBaseOf<TerminationCriterion>,
                FGlue::IsNotBaseOf<Step>
              > > , FGlue::Compose>,
              Mixin::RelativeAccuracy, Mixin::AbsoluteAccuracy, Mixin::MinimalAccuracy, Mixin::Eps
          >,
          FGlue::Apply< FGlue::StoreIf< FGlue::IsNotDerivedFrom<Step> > , Mixin::Verbosity >
        >
    {
    public:
      /*!
        @brief Construct from given step implementation and termination criterions.
        @param step object implementing one step of an iterative scheme
        @param terminate termination criterion
        @param maxSteps
       */
      IterativeMethod(Step step, TerminationCriterion terminate, unsigned maxSteps = 1000)
        : Step(std::move(step)) ,
          Mixin::MaxSteps(maxSteps) ,
          terminate_(std::move(terminate))
      {
        initializeConnections();
      }

      IterativeMethod(const IterativeMethod& other)
        : Step(static_cast<const Step&>(other)),
          Mixin::MaxSteps(other),
          terminate_(other.terminate_)
      {
        initializeConnections();
      }

      IterativeMethod(IterativeMethod&& other)
        : Step(std::move(static_cast<Step&&>(other))),
          Mixin::MaxSteps(std::move(other)),
          terminate_(std::move(other.terminate_))
      {
        initializeConnections();
      }

      IterativeMethod& operator=(const IterativeMethod& other)
      {
        Step::operator=(static_cast<const Step&>(other));
        Mixin::MaxSteps::operator=(other);
        terminate_ = other.terminate_;
        initializeConnections();
      }

      IterativeMethod& operator=(IterativeMethod&& other)
      {
        Step::operator=(std::move(static_cast<Step&&>(other)));
        Mixin::MaxSteps::operator=(std::move(other));
        terminate_ = std::move(other.terminate_);
        initializeConnections();
      }

      /*!
        @brief Apply iterative method to solve \f$Ax=b\f$.
        @param x initial iterate
        @param b initial right hand side
        @param res some statistics
       */
      Vector operator()(Vector x, Vector b)
      {
        if( this->verbosityLevel() > 1) std::cout << "\n === " << Step::name() << " === " << std::endl;

        init(x,b);

        auto step=1u;
        for(; step<=maxSteps(); ++step)
        {
          Step::compute(x,b);

          if( terminate_ || Optional::terminate(*this) )
            break;

          if( Optional::restart(*this) )
            reset(step,x,b);
        }

        if( step > maxSteps() )
          throw Exception::NotConverged(Step::name());

        Step::postProcess(x);

        return x;
      }

//      /*!
//        @brief Apply iterative method to solve \f$Ax=b\f$.
//        @param x initial iterate
//        @param b initial right hand side
//        @param relativeAccuracy required relative accuracy
//       */
//      Vector operator()(Vector x, Vector b, double relativeAccuracy)
//      {
//        terminate_.setRelativeAccuracy(relativeAccuracy);
//        return (*this)(std::move(x),std::move(b));
//      }

//      Vector operator()(Vector b)
//      {
//        return (*this)(step.domain().zeroVector(),b);
//      }

      //! Access termination criterion.
      TerminationCriterion& getTerminationCriterion()
      {
        return terminate_;
      }

    private:
      void init(Vector& x, Vector& b)
      {
        storeInitialInput(x,b);
        Step::init(x,b);
        terminate_.init();
      }

      void reset(unsigned& step, Vector& x, Vector& b)
      {
        restoreInitialInput(x,b);
        Step::reset(x,b);
        terminate_.init();
        step = 0u;
      }

      void initializeConnections()
      {
        terminate_.connect(*this);
        Optional::bind_connect_minimalDecreaseAchieved(terminate_,*this);

        using namespace Mixin;
        Optional::Mixin::Attach< AbsoluteAccuracy, MinimalAccuracy, RelativeAccuracy , Verbosity , Eps >::apply(*this,terminate_);
      }

      void storeInitialInput(const Vector& x, const Vector& b)
      {
        x0_ = x;
        b0_ = b;
      }

      void restoreInitialInput(Vector& x, Vector& b)
      {
        x = x0_;
        b = b0_;
      }

      TerminationCriterion terminate_;
      Vector x0_;
      Vector b0_;
    };

    template <class Step, class TerminationCriterion>
    auto makeIterativeMethod(Step&& step, TerminationCriterion&& terminationCriterion)
    {
      return IterativeMethod<std::decay_t<Step>,std::decay_t<TerminationCriterion>>(std::forward<Step>(step),std::forward<TerminationCriterion>(terminationCriterion));
    }
  }
  /** @} */
}

#endif // SPACY_ALGORITHM_GENERIC_ITERATIVE_METHOD_HH
