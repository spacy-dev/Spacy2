// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACYS_UTIL_CAST_TO_HH
#define SPACYS_UTIL_CAST_TO_HH

#include "Spacy/Util/voider.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

namespace Spacy
{
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
    return x.template target<ToType>() != nullptr;
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'FromType&' to 'ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<ToType&>(x)
   */
  template < class ToType , class FromType , class = std::enable_if_t<!std::is_same<FromType,::Spacy::Vector>::value> >
  ToType& cast_ref(FromType& x)
  {
    return *x.template target<ToType>();
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'const FromType&' to 'const ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<const ToType&>(x)
   */
  template < class ToType , class FromType , class = std::enable_if_t<!std::is_same<FromType,::Spacy::Vector>::value> >
  const ToType& cast_ref(const FromType& x)
  {
    return *x.template target<ToType>();
  }
}

#endif // SPACYS_UTIL_CAST_TO_HH
