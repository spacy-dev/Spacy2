#ifndef FGLUE_TMP_CONSTANT_HH
#define FGLUE_TMP_CONSTANT_HH

namespace FGlue
{
  template <class Operation>
  struct Constant
  {
    template <class...Args>
    struct apply
    {
      using type = Operation;
    };
  };
}

#endif // FGLUE_TMP_CONSTANT_HH
