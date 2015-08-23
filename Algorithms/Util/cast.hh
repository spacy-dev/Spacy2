#ifndef ALGORITHMS_UTIL_CAST_TO_HH
#define ALGORITHMS_UTIL_CAST_TO_HH

#include <boost/type_erasure/any_cast.hpp>

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
//  template < class ToType , class FromType >
//  bool is(const FromType& x)
//  {
//    return dynamic_cast< const ToType* >(&x) != nullptr;
//  }

//  template <class ToType, class FromType>
//  ToType& castTo(FromType& x)
//  {
//    if( !is<ToType>(x) ) throw InvalidArgumentException("castTo");
//    return dynamic_cast<ToType&>(x);
//  }

//  template <class ToType, class FromType>
//  const ToType& castTo(const FromType& x)
//  {
//    if( !is<ToType>(x) ) throw InvalidArgumentException("castTo");
//    return dynamic_cast<const ToType&>(x);
//  }

  template < class ToType , class FromType >
  bool isAny(const FromType& x)
  {
    return boost::type_erasure::any_cast< const std::decay_t<ToType>* >(&x) != nullptr;
  }

  template <class ToType, class FromType>
  ToType& cast_ref(FromType& x)
  {
    if( !isAny<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<ToType&>(x);
  }

  template <class ToType, class FromType>
  const ToType& cast_ref(const FromType& x)
  {
    if( !isAny<ToType>(x) ) throw InvalidArgumentException("cast_ref");
    return boost::type_erasure::any_cast<const ToType&>(x);
  }
}

#endif // ALGORITHMS_UTIL_CAST_TO_HH
