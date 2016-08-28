#ifndef FGLUE_TMP_STORE_IF_HH
#define FGLUE_TMP_STORE_IF_HH

#include <type_traits>
#include "apply.hh"
#include "empty.hh"
#include "isBaseOf.hh"
#include "isDerivedFrom.hh"
#include "trueFalse.hh"

namespace FGlue
{
  //! Stores Arg if Operation::template apply<Arg>::type evaluates to True, else stores Empty.
  template <class Operation>
  struct StoreIf
  {
    template <class Arg>
    struct apply
    {
      using type = typename std::conditional< isTrue< Apply<Operation,Arg> >() , Arg , Empty>::type;
    };
  };
}

#endif // FGLUE_TMP_STORE_IF_HH
