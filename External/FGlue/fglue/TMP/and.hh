#ifndef GLUE_TMP_AND_HH
#define GLUE_TMP_AND_HH

#include <type_traits>
#include "trueFalse.hh"

namespace FGlue
{
  //! Logical "and" for meta-functions. Return True or False.
  class And
  {
  public:
    template <class First, class Second>
    struct apply
    {
       using type = std::conditional_t< isTrue<First>() && isTrue<Second>() , std::true_type , std::false_type >;
    };
  };
}

#endif // GLUE_TMP_AND_HH
