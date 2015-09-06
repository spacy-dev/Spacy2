#ifndef SPACYS_UTIL_CAST_TO_HH
#define SPACYS_UTIL_CAST_TO_HH

#include <boost/type_erasure/any_cast.hpp>

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief Check if x can be cast to a reference of type ToType.
   * @return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr
   */
  template < class ToType , class FromType >
  bool is(const FromType& x)
  {
    return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr;
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'FromType&' to 'ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<ToType&>(x)
   */
  template <class ToType, class FromType>
  ToType& cast_ref(FromType& x)
  {
    if( !is<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<ToType&>(x);
  }

  /**
   * @ingroup SpacyGroup
   * @brief cast x of type 'const FromType&' to 'const ToType&' with boost::type_erasure::any_cast
   * @return boost::type_erasure::any_cast<const ToType&>(x)
   */
  template <class ToType, class FromType>
  const ToType& cast_ref(const FromType& x)
  {
    if( !is<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<const ToType&>(x);
  }
}

#endif // SPACYS_UTIL_CAST_TO_HH
