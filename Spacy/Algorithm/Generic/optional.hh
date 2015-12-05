#ifndef SPACY_ALGORITHM_GENERIC_OPTIONAL_HH
#define SPACY_ALGORITHM_GENERIC_OPTIONAL_HH

#include <type_traits>
#include <utility>
#include "Spacy/Util/voider.hh"

namespace Spacy
{
  //! @cond
  namespace Try
  {
    template <class Type>
    using MemFn_terminate = decltype( std::declval<Type>().terminate() );

    template <class ToConnect, class Connector>
    using MemFn_connect = decltype(std::declval<Connector>().connect(std::declval<ToConnect>()));

    template <class Type>
    using MemFn_minimalDecreaseAchieved = decltype( std::declval<Type>().minimalDecreaseAchieved() );

    template <class Type>
    using MemFn_restart = decltype(std::declval<Type>().restart());
  }
  //! @endcond

  /*!
    Functions in this namespace use SFINAE to check if functions, that are to be executed, actually exist.
    If this is not the case fallback implementations are provided, i.e. in general nothing happens then.
   */
  namespace Optional
  {
    //! @cond
    namespace Detail
    {
      template <class Type, class = void>
      struct BindMemFn_MinimalDecreaseAchieved
      {
        static std::function<bool()> apply(const Type&)
        {
          return std::function<bool()>{};
        }
      };

      template <class Type>
      struct BindMemFn_MinimalDecreaseAchieved< Type , void_t< Try::MemFn_minimalDecreaseAchieved<Type> > >
      {
        static std::function<bool()> apply(const Type& t)
        {
          return std::bind(&Type::minimalDecreaseAchieved,&t);
        }
      };

      template <class ToConnect, class Connector, class = void>
      struct Connect
      {
        static void apply(const ToConnect&, Connector&)
        {}
      };

      template <class ToConnect, class Connector>
      struct Connect< ToConnect , Connector , void_t<Try::MemFn_connect<ToConnect,Connector> > >
      {
        static void apply(const ToConnect& toConnect, Connector& connector)
        {
          connector.connect(toConnect);
        }
      };


      template <class Type, class = void>
      struct Restart
      {
        static bool apply(const Type&)
        {
          return false;
        }
      };

      template <class Type>
      struct Restart< Type , void_t< Try::MemFn_restart<Type> > >
      {
        static bool apply(const Type& t)
        {
          return t.restart();
        }
      };


      template <class Type, class = void>
      struct Terminate
      {
        static bool apply(const Type&)
        {
          return false;
        }
      };

      template <class Type>
      struct Terminate< Type , void_t< Try::MemFn_terminate<Type> > >
      {
        static bool apply(const Type& t)
        {
          return t.terminate();
        }
      };
    }
    //! @endcond


    template <class Type>
    bool terminate(const Type& t)
    {
      return Detail::Terminate<Type>::apply(t);
    }


    template <class Type>
    bool restart(const Type& t)
    {
      return Detail::Restart<Type>::apply(t);
    }


    template <class ToConnect, class Connector>
    void connect(const ToConnect& toConnect, Connector& connector)
    {
      Detail::Connect<ToConnect,Connector>::apply(toConnect,connector);
    }


    template <class Type>
    auto bind_minimalDecreaseAchieved(const Type& t) -> decltype( Detail::BindMemFn_MinimalDecreaseAchieved<Type>::apply(t) )
    {
      return Detail::BindMemFn_MinimalDecreaseAchieved<Type>::apply(t);
    }


    template <class ToConnect, class Connector>
    void bind_connect_minimalDecreaseAchieved(const ToConnect& toConnect, Connector& connector)
    {
      auto relaxedTerminationCriterion = bind_minimalDecreaseAchieved(toConnect);
      if( relaxedTerminationCriterion )
        connect(relaxedTerminationCriterion,connector);
    }
  }
}

#endif // SPACY_ALGORITHM_GENERIC_OPTIONAL_HH
