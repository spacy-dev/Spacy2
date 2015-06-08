#ifndef ALGORITHM_UTIL_STATIC_CHECKS_HH
#define ALGORITHM_UTIL_STATIC_CHECKS_HH

#include <utility>

#include "voider.hh"

namespace Algorithm
{
  template <class Vector> using TryCall_FreeFunction_Dot = decltype( dot( std::declval<Vector>() , std::declval<Vector>() ) );

  template <class Vector, class = void> struct Has_FreeFunction_Dot : std::false_type {};
  template <class Vector>
  struct Has_FreeFunction_Dot< Vector , void_t<TryCall_FreeFunction_Dot> > : std::true_type {};
}

#endif // ALGORITHM_UTIL_STATIC_CHECKS_HH
