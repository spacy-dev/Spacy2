#ifndef FGLUE_TMP_IS_DERIVED_FROM_HH
#define FGLUE_TMP_IS_DERIVED_FROM_HH

#include <type_traits>

#include "apply.hh"
#include "chain.hh"
#include "not.hh"

namespace FGlue
{
  //! Meta-function that checks if its argument is derived from Base.
  template <class Base>
  struct IsDerivedFrom
  {
    template <class Derived>
    struct apply
    {
      using type = std::conditional_t<std::is_base_of<Base,Derived>::value,True,False>;
    };
  };

  //! Meta-function that checks if its argument is not derived from Base
  template <class Base>
  using IsNotDerivedFrom = Apply< Chain , Not , IsDerivedFrom<Base> >;
}

#endif // FGLUE_TMP_IS_DERIVED_FROM_HH

