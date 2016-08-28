#ifndef FGLUE_TMP_MAKE_HH
#define FGLUE_TMP_MAKE_HH

#include "apply.hh"

namespace FGlue
{
  template <template <class...> class Operation>
  struct Make
  {
    template <class... Args>
    struct apply
    {
      using type = Operation<Args...>;
    };
  };
}

#endif // FGLUE_TMP_MAKE_HH
