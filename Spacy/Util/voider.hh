// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_VOIDER_HH
#define SPACY_UTIL_VOIDER_HH

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
  template <class... Types>
  using void_t = typename Detail::voider<Types...>::type;


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
  using Require = typename Detail::Require<Concept>::type;
}

#endif // SPACY_UTIL_VOIDER_HH
