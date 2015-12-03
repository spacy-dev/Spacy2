// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MEM_FN_CHECKS_HH
#define SPACY_UTIL_MEM_FN_CHECKS_HH

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /// @cond
  class LinearOperator;
  class VectorSpace;
  class Vector;
  /// @endcond

  template <class TypeErased, class Type>
  using isConstructibleOrSame = std::integral_constant< bool , std::is_constructible< TypeErased , Type >::value || std::is_same< TypeErased , Type >::value>;

  template <class T>
  using TryMemFn_space = decltype(std::declval<T>().space());

  template <class T>
  using TryMemFn_domain = decltype(std::declval<T>().domain());

  template <class T>
  using TryMemFn_range = decltype(std::declval<T>().range());

  template <class T, class... Args>
  using TryMemFn_d1_Functional = decltype(std::declval<T>().d1(std::declval<Args>()...));

  template <class T, class V>
  using TryMemFn_d1_Operator = decltype(std::declval<T>().d1(std::declval<V>(),std::declval<V>()));

  template <class T, class V>
  using TryMemFn_d1_DynamicOperator = decltype(std::declval<T>().d1(0.,std::declval<V>(),std::declval<V>()));

  template <class T, class V>
  using TryMemFn_d2_Functional = decltype(std::declval<T>().d2(std::declval<V>(),std::declval<V>()));

  template <class T, class V>
  using TryMemFn_hessian = decltype(std::declval<T>().hessian(std::declval<V>()));

  template <class T, class... Args>
  using TryMemFn_linearization = decltype(std::declval<T>().linearization(std::declval<Args>()...));

  template <class T>
  using TryMemFn_M = decltype(std::declval<T>().M());

  template <class T>
  using TryMemFn_solver = decltype( std::declval<T>().solver() );

  template <class T>
  using TryMemFn_isPositiveDefinite = decltype(std::declval<T>().isPositiveDefinite());

  template <class T>
  using TryMemFn_clone = decltype(std::declval<T>().clone());

  template <class T>
  using TryMemFn_toFile = decltype(std::declval<T>().toFile(std::declval<std::string>()));

  template <class T>
  using TryMemFn_clear = decltype(std::declval<T>().clear());

  template <class T>
  using TryMemFn_update = decltype(std::declval<T>().update(1.,1.,1.,1.));

  template <class T>
  using TryMemFn_vanishingStep = decltype(std::declval<T>().vanishingStep());

  template <class T>
  using TryMemFn_minimalDecreaseAchieved = decltype(std::declval<T>().minimalDecreaseAchieved());

  template <class T>
  using TryMemFn_setEps = decltype(std::declval<T>().setEps(1.));

  template <class T>
  using TryMemFn_setRelativeAccuracy = decltype(std::declval<T>().setRelativeAccuracy(1.));

  template <class T>
  using TryMemFn_setAbsoluteAccuracy = decltype(std::declval<T>().setAbsoluteAccuracy(1.));

  template <class T>
  using TryMemFn_setMinimalAccuracy = decltype(std::declval<T>().setMinimalAccuracy(1.));


  template <class,class=void>
  struct HasMemFn_space : std::false_type{};

  template <class T>
  struct HasMemFn_space< T , void_t< TryMemFn_space<T> > >
      : std::is_same< const VectorSpace& ,TryMemFn_space<T> >::type
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
      : isConstructibleOrSame< LinearOperator , TryMemFn_hessian<T,V> >
  {};

  template <class,class,class=void>
  struct HasMemFn_linearization : std::false_type{};

  template <class T,class V>
  struct HasMemFn_linearization< T , V , void_t< TryMemFn_linearization<T,V> > >
      : isConstructibleOrSame< LinearOperator , TryMemFn_linearization<T,V> >
  {};

  template <class,class=void>
  struct HasMemFn_M : std::false_type{};

  template <class T>
  struct HasMemFn_M< T , void_t< TryMemFn_M<T> > >
      : isConstructibleOrSame< LinearOperator , TryMemFn_M<T> >
  {};

  template <class,class=void>
  struct HasMemFn_solver : std::false_type{};

  template <class T>
  struct HasMemFn_solver< T , void_t< TryMemFn_solver<T> > >
      : isConstructibleOrSame< std::function<Vector(const Vector&)> , TryMemFn_solver<T> >
  {};

  template <class,class=void>
  struct HasMemFn_isPositiveDefinite : std::false_type{};

  template <class T>
  struct HasMemFn_isPositiveDefinite< T , void_t< TryMemFn_isPositiveDefinite<T> > >
      : std::is_same< bool , TryMemFn_isPositiveDefinite<T> >
  {};

  template <class T>
  using HasMemFn_clear = typename IsVoid<T>::template apply<TryMemFn_clear>;

  template <class T>
  using HasMemFn_update = typename IsVoid<T>::template apply<TryMemFn_update>;

  template <class,class=void>
  struct HasMemFn_vanishingStep : std::false_type{};

  template <class T>
  struct HasMemFn_vanishingStep< T , void_t< TryMemFn_vanishingStep<T> > >
      : std::is_same< bool , TryMemFn_vanishingStep<T> >
  {};

  template <class,class=void>
  struct HasMemFn_minimalDecreaseAchieved : std::false_type{};

  template <class T>
  struct HasMemFn_minimalDecreaseAchieved< T , void_t< TryMemFn_minimalDecreaseAchieved<T> > >
      : std::is_same< bool , TryMemFn_minimalDecreaseAchieved<T> >
  {};

  template <class T>
  using HasMemFn_setEps = typename IsVoid<T>::template apply<TryMemFn_setEps>;

  template <class T>
  using HasMemFn_setRelativeAccuracy = typename IsVoid<T>::template apply<TryMemFn_setRelativeAccuracy>;

  template <class T>
  using HasMemFn_setAbsoluteAccuracy = typename IsVoid<T>::template apply<TryMemFn_setAbsoluteAccuracy>;

  template <class T>
  using HasMemFn_setMinimalAccuracy = typename IsVoid<T>::template apply<TryMemFn_setMinimalAccuracy>;
}

#endif // SPACY_UTIL_MEM_FN_CHECKS_HH
