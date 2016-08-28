#ifndef FGLUE_TMP_CHAIN_HH
#define FGLUE_TMP_CHAIN_HH

#include "apply.hh"

namespace FGlue
{
  struct Chain
  {
    template <class F, class G>
    struct apply
    {
      struct type
      {
        template <class... Args>
        struct apply
        {
          using type = Apply< F , Apply<G,Args...> >;
        };
      };
    };
  };
}

#endif // FGLUE_TMP_CHAIN_HH
