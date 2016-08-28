#ifndef FGLUE_TMP_NOT_HH
#define FGLUE_TMP_NOT_HH

#include <type_traits>
#include "trueFalse.hh"

namespace FGlue
{
  //! Logical "not" for meta-functions.
  struct Not
  {
    template <class Arg>
    struct apply
    {
      using type = std::conditional_t< isTrue<Arg>() , False , True >;
    };
  };
}

#endif // FGLUE_TMP_NOT_HH
