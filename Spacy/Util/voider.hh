#pragma once

namespace Spacy
{
  /// @cond
  namespace Detail
  {
    /// Helper, required in order to make void_t work with gcc.
    template <class...> struct voider { using type = void; };

    template <class Concept>
    struct Require
    {
      static_assert( Concept::value , "Concept not satisfied." );
      using type = void;
    };
  }
  /// @endcond


  /// Most fascinating type ever. Is always void.
  template <class...>
  using void_t = void;//typename Detail::voider<Types...>::type;


  template <class... Args>
  struct IsVoid
  {
    template <template <class...> class Operation, class = void>
    struct apply : std::false_type
    {};

    template <template <class...> class Operation>
    struct apply< Operation, void_t< Operation<Args...> > > : std::true_type
    {};
  };

  /// Check if Concept is satisfied.
  template <class Concept>
  struct Require
  {
    static_assert( Concept::value , "Concept failed" );
  };
}
