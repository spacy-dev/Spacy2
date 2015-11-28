// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MEM_OP_CHECKS_HH
#define SPACY_UTIL_MEM_OP_CHECKS_HH

#include <type_traits>
#include <utility>
#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  template <class T>
  using TryMemOp_add = decltype(std::declval<T>() += std::declval<T>());

  template <class T>
  using TryMemOp_subtract = decltype(std::declval<T>() -= std::declval<T>());

  template <class T>
  using TryMemOp_multiply = decltype(std::declval<T>() *= 1.);

  template <class T>
  using TryMemOp_negate = decltype(std::declval<T>().operator-());

  template <class T, class V>
  using TryMemOp_callable = decltype(std::declval<T>()(std::declval<V>()));


  template <class,class=void>
  struct HasMemOp_add : std::false_type{};

  template <class T>
  struct HasMemOp_add< T , void_t< TryMemOp_add<T> > >
    : std::is_same< T& ,TryMemOp_add<T> >::type
  {};

  template <class,class=void>
  struct HasMemOp_subtract : std::false_type{};

  template <class T>
  struct HasMemOp_subtract< T , void_t< TryMemOp_subtract<T> > >
    : std::is_same< T& ,TryMemOp_subtract<T> >::type
  {};

  template <class,class=void>
  struct HasMemOp_multiply : std::false_type{};

  template <class T>
  struct HasMemOp_multiply< T , void_t< TryMemOp_multiply<T> > >
    : std::is_same< T& ,TryMemOp_multiply<T> >::type
  {};

  template <class,class=void>
  struct HasMemOp_negate : std::false_type{};

  template <class T>
  struct HasMemOp_negate< T , void_t< TryMemOp_negate<T> > >
    : std::is_same< T ,TryMemOp_negate<T> >::type
  {};

  template <class T, class Arg, class Return=Arg, class=void>
  struct HasMemOp_callable : std::false_type{};

  template <class T,class Arg, class Return>
  struct HasMemOp_callable< T , Arg , Return , void_t< TryMemOp_callable<T,Arg> > >
    : std::is_same< Return ,TryMemOp_callable<T,Arg> >::type
  {};
}

#endif // SPACY_UTIL_MEM_OP_CHECKS_HH
