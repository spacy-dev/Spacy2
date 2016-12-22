#ifndef SPACY_ALGORITHM_GENERIC_STEP_HH
#define SPACY_ALGORITHM_GENERIC_STEP_HH

#include "Spacy/vector.hh"
#include "Spacy/Util/mixins.hh"
#include "Spacy/Spaces/ScalarSpace/Real.hh"
#include "fglue/TMP/createMissingBaseClasses.hh"
#include "fglue/Fusion/connect.hh"

namespace Spacy
{
  /** @addtogroup GenericGroup @{ */
  namespace Generic
  {
    struct NoData
    {
      Vector iterate() const
      {
        return Real(0);
      }
    };

    template <class Data>
    struct DefaultInterface
    {
      DefaultInterface() = default;

      template <class T,
                class = std::enable_if_t<std::is_same< std::decay_t<T> , Data >::value> >
      DefaultInterface(T&& t)
        : data_(std::forward<T>(t))
      {}

      template <class T,
                class = std::enable_if_t<std::is_same< std::decay_t<T> , Data >::value> >
      DefaultInterface& operator=(T&& t)
      {
        data_ = std::forward<T>(t);
      }

      Data data_;
    };

    struct Ignore
    {
      template <class... Args>
      void operator()(Args&&...) const {}

      template <class Arg>
      void post(Arg&&) const {}
    };

    /*!
      @brief Generic step of an iterative method.

      Solves a linear operator equation \f$Ax=b\f$ with \f$A:\ X\mapsto Y\f$, resp. one of its preconditioned versions
      \f$PAx=Px\f$ or \f$P_1AP_2P_2^{-1}x=P_1b\f$.

        The following substeps are performed and may be specified.
         1. Apply preconditioner
         2. Compute search direction
         3. Compute scaling for the search direction
         4. Update iterate
     */
    template <class ApplyPreconditioner = Ignore,
              class SearchDirection = Ignore ,
              class Scaling = Ignore ,
              class UpdateIterate = Ignore ,
              class Data = NoData ,
              template <class> class Interface = DefaultInterface>
    class Step :
        public Interface<Data>,
        public FGlue::EnableBaseClassesIf<
            FGlue::IsBaseOfOneOf<ApplyPreconditioner,SearchDirection,Scaling,UpdateIterate,Data>,
            Mixin::IterativeRefinements , Mixin::Verbosity , Mixin::Eps
        >
    {
      using Interface<Data>::data_;
    public:
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Interface<Data>,Args...>::value> >
      Step(Args&&... args)
        : Interface<Data>(std::forward<Args>(args)...)
      {
        initializeConnections();
      }

      Step(const Step& other)
        : Interface<Data>(other.data_)
      {
        initializeConnections();
      }

      Step& operator=(const Step& other)
      {
        Interface<Data>::operator=(other.data_);
        initializeConnections();
      }

      Step& operator=(Step&& other)
      {
        Interface<Data>::operator=(std::move(other.data_));
        initializeConnections();
      }

      Step(Step&& other)
        : Interface<Data>(std::move(other.data_))
      {
        initializeConnections();
      }

      /*!
        @param x initial iterate
        @param b initial right hand side
       */
      void init(Vector& x, Vector& b)
      {
        applyPreconditioner_.pre(data_,x,b);
        data_.init(x,b);
      }

      void reset(Vector& x, Vector& b)
      {
        data_.reset(x,b);
      }

      /*!
        @brief Perform one step of an iterative method.

        @param x current iterate
        @param b current right hand side
       */
      void compute()
      {
        applyPreconditioner_(data_);
        computeSearchDirection_(data_);
        computeScaling_(data_);
        updateIterate_(data_);
      }

      /// Access and possibly post-process final iterate, i.e. apply @code{.cpp} P_.post(x) @endcode
      Vector getFinalIterate()
      {
        applyPreconditioner_.post(data_);
        return data_.iterate();
      }

    private:
      void initializeConnections()
      {
        FGlue::Connector< FGlue::IsDerivedFrom<Mixin::IterativeRefinements> >::template
            from<Mixin::IterativeRefinements>(*this).to
            (applyPreconditioner_,computeSearchDirection_,computeScaling_,updateIterate_,data_);
        FGlue::Connector< FGlue::IsDerivedFrom<Mixin::Verbosity> >::template
            from<Mixin::Verbosity>(*this).
            to(applyPreconditioner_,computeSearchDirection_,computeScaling_,updateIterate_,data_);
        FGlue::Connector< FGlue::IsDerivedFrom<Mixin::Eps> >::template
            from<Mixin::Eps>(*this).
            to(applyPreconditioner_,computeSearchDirection_,computeScaling_,updateIterate_,data_);
      }

      ApplyPreconditioner applyPreconditioner_;
      SearchDirection computeSearchDirection_;
      Scaling computeScaling_;
      UpdateIterate updateIterate_;
    };
  }
  /** @} */
}

#endif // SPACY_ALGORITHM_GENERIC_STEP_HH
