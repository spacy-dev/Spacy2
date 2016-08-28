#ifndef FGLUE_TMP_IS_BASE_OF_HH
#define FGLUE_TMP_IS_BASE_OF_HH

#include <type_traits>

#include "apply.hh"
#include "chain.hh"
#include "not.hh"
#include "trueFalse.hh"
#include "variadic.hh"

namespace FGlue
{
  //! Meta-function that checks if its argument is a base class of Derived.
  template <class Derived>
  struct IsBaseOf
  {
    template <class Base>
    struct apply
    {
      using type = std::conditional_t<std::is_base_of<Base,Derived>::value,True,False>;
    };
  };

  //! Meta-function that checks if its argument is not a base class of Derived.
  template <class Derived>
  using IsNotBaseOf = Apply< Chain , Not , IsBaseOf<Derived> >;
}

#endif // FGLUE_TMP_IS_BASE_OF_HH
