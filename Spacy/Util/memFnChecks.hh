// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MEM_FN_CHECKS_HH
#define SPACY_UTIL_MEM_FN_CHECKS_HH

#include <type_traits>
#include <utility>
#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond


  template <class T>
  using TryMemFn_space = decltype(std::declval<T>().space());

  template <class T>
  using TryMemFn_domain = decltype(std::declval<T>().domain());

  template <class T>
  using TryMemFn_range = decltype(std::declval<T>().range());

  template <class T, class V>
  using TryMemFn_d1_Functional = decltype(std::declval<T>().d1(std::declval<V>()));

  template <class T, class V>
  using TryMemFn_d1_Operator = decltype(std::declval<T>().d1(std::declval<V>(),std::declval<V>()));

  template <class T, class V>
  using TryMemFn_d2_Functional = decltype(std::declval<T>().d2(std::declval<V>(),std::declval<V>()));

  template <class T, class V>
  using TryMemFn_hessian = decltype(std::declval<T>().hessian(std::declval<V>()));

  template <class T, class V>
  using TryMemFn_linearization = decltype(std::declval<T>().linearization(std::declval<V>()));

  template <class T>
  using TryMemFn_solver = decltype( std::declval<T>().solver() );


  template <class,class=void>
  struct HasMemFn_space : std::false_type{};

  template <class T>
  struct HasMemFn_space< T , void_t< TryMemFn_space<T> > >
    : std::is_same< const VectorSpace* ,TryMemFn_space<T> >::type
  {};

  template <class,class=void>
  struct HasMemFn_domain : std::false_type{};

  template <class T>
  struct HasMemFn_domain< T , void_t< TryMemFn_domain<T> > >
    : std::is_same< const VectorSpace& ,TryMemFn_domain<T> >::type
  {};

  template <class,class=void>
  struct HasMemFn_range : std::false_type{};

  template <class T>
  struct HasMemFn_range< T , void_t< TryMemFn_range<T> > >
    : std::is_same< const VectorSpace& ,TryMemFn_range<T> >::type
  {};

  template <class,class,class=void>
  struct HasMemFn_d1_Functional : std::false_type{};

  template <class T,class V>
  struct HasMemFn_d1_Functional< T , V , void_t< TryMemFn_d1_Functional<T,V> > >
    : std::is_same< V ,TryMemFn_d1_Functional<T,V> >::type
  {};

  template <class,class,class=void>
  struct HasMemFn_d1_Operator : std::false_type{};

  template <class T,class V>
  struct HasMemFn_d1_Operator< T , V , void_t< TryMemFn_d1_Operator<T,V> > >
    : std::is_same< V ,TryMemFn_d1_Operator<T,V> >::type
  {};

  template <class,class,class=void>
  struct HasMemFn_d2_Functional : std::false_type{};

  template <class T,class V>
  struct HasMemFn_d2_Functional< T , V , void_t< TryMemFn_d2_Functional<T,V> > >
    : std::is_same< V ,TryMemFn_d2_Functional<T,V> >::type
  {};

  template <class,class,class=void>
  struct HasMemFn_hessian : std::false_type{};

  template <class T,class V>
  struct HasMemFn_hessian< T , V , void_t< TryMemFn_hessian<T,V> > >
  : std::true_type
      //    : std::is_same< V ,TryMemFn_hessian<T,V> >::type
  {};

  template <class,class,class=void>
  struct HasMemFn_linearization : std::false_type{};

  template <class T,class V>
  struct HasMemFn_linearization< T , V , void_t< TryMemFn_linearization<T,V> > >
  : std::true_type
      //    : std::is_same< V ,TryMemFn_linearization<T,V> >::type
  {};

  template <class,class=void>
  struct HasMemFn_solver : std::false_type{};

  template <class T>
  struct HasMemFn_solver< T , void_t< TryMemFn_solver<T> > >
  : std::true_type
      //    : std::is_same< V ,TryMemFn_solver<T,V> >::type
  {};
}

#endif // SPACY_UTIL_MEM_FN_CHECKS_HH
