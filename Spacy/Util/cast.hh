// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACYS_UTIL_CAST_TO_HH
#define SPACYS_UTIL_CAST_TO_HH

#include <boost/type_erasure/any_cast.hpp>

#include "Spacy/Util/voider.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

namespace Spacy
{
  namespace Detail
  {
    template <class TypeErased, class Target>
    using TryMemFn_Target = decltype(std::declval<TypeErased>().template target<Target>());

    template <class ToType, class FromType, class = void>
    struct CastHelper
    {
      static bool castable(const FromType& x)
      {
        return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr;
      }

      static ToType& cast(FromType& x)
      {
        if( !castable(x) ) throw InvalidArgumentException("anycast");
        return boost::type_erasure::any_cast<ToType&>(x);
      }

      static const ToType& cast(const FromType& x)
      {
        if( !castable(x) ) throw InvalidArgumentException("anycast");
        return boost::type_erasure::any_cast<const ToType&>(x);
      }
    };

    template <class ToType, class FromType>
    struct CastHelper< ToType , FromType , void_t< TryMemFn_Target<FromType,ToType> > >
    {
      static bool castable(const FromType& x)
      {
        return x.template target<ToType>() != nullptr;
      }

      static ToType& cast(FromType& x)
      {
        if( !castable(x) ) throw InvalidArgumentException("target");
        return *x.template target<ToType>();
      }

      static const ToType& cast(const FromType& x)
      {
        if( !castable(x) ) throw InvalidArgumentException("target");
        return *x.template target<ToType>();
      }
    };
  }

  /// @cond
  class Vector;
  /// @endcond

  /**
   * @ingroup SpacyGroup
   * @brief Check if x can be cast to a reference of type ToType.
   * @return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr
   */
  template < class ToType , class FromType , class = std::enable_if_t<!std::is_same<FromType,::Spacy::Vector>::value> >
  bool is(const FromType& x)
  {
    return Detail::CastHelper<ToType,FromType>::castable(x);
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'FromType&' to 'ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<ToType&>(x)
   */
  template < class ToType , class FromType , class = std::enable_if_t<!std::is_same<FromType,::Spacy::Vector>::value> >
  ToType& cast_ref(FromType& x)
  {
    return Detail::CastHelper<ToType,FromType>::cast(x);
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'const FromType&' to 'const ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<const ToType&>(x)
   */
  template < class ToType , class FromType , class = std::enable_if_t<!std::is_same<FromType,::Spacy::Vector>::value> >
  const ToType& cast_ref(const FromType& x)
  {
    return Detail::CastHelper<ToType,FromType>::cast(x);
  }
}

#endif // SPACYS_UTIL_CAST_TO_HH
