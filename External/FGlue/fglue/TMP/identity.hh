#ifndef FGLUE_TMP_IDENTITY_HH
#define FGLUE_TMP_IDENTITY_HH

namespace FGlue
{
  struct Identity
  {
    template <class Arg>
    struct apply
    {
      using type = Arg;
    };
  };
}

#endif // FGLUE_TMP_IDENTITY_HH
