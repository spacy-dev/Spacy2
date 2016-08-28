#ifndef GLUE_TMP_OR_HH
#define GLUE_TMP_OR_HH

#include <type_traits>
#include "trueFalse.hh"

namespace FGlue
{
  //! Logical "or" for meta-functions that return std::true_type or std::false_type.
  struct Or
  {
    template <class First, class Second>
    struct apply
    {
      using type = std::conditional_t< isTrue<First>() || isTrue<Second>() , True , False >;
    };
  };
}

#endif // GLUE_TMP_OR_HH
