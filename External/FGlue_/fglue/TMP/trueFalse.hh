#ifndef FGLUE_TRUE_FALSE_HH
#define FGLUE_TRUE_FALSE_HH

#include <type_traits>

namespace FGlue
{
  //! Template meta-function that always evaluates to std::true_type.
  using True = std::true_type;

  //! Template meta-function that always evaluates to std::false_type.
  using False = std::false_type;

  //! @return std::is_same<std::decay_t<Type>,True>::value;
  template <class Type>
  constexpr bool isTrue()
  {
    return std::is_same<std::decay_t<Type>,True>::value;
  }
}

#endif // FGLUE_TRUE_FALSE_HH
