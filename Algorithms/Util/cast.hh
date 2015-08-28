#ifndef ALGORITHMS_UTIL_CAST_TO_HH
#define ALGORITHMS_UTIL_CAST_TO_HH

#include <boost/type_erasure/any_cast.hpp>

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  template < class ToType , class FromType >
  bool is(const FromType& x)
  {
    return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr;
  }

  /**
   * @ingroup VHatGroup
   * @brief cast x of type 'FromType&' to 'ToType&' with boost::type_erasure::any_cast.
   */
  template <class ToType, class FromType>
  ToType& cast_ref(FromType& x)
  {
    if( !is<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<ToType&>(x);
  }

  /**
   * @ingroup VHatGroup
   * @brief cast x of type 'const FromType&' to 'const ToType&' with boost::type_erasure::any_cast.
   */
  template <class ToType, class FromType>
  const ToType& cast_ref(const FromType& x)
  {
    if( !is<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<const ToType&>(x);
  }
}

#endif // ALGORITHMS_UTIL_CAST_TO_HH
