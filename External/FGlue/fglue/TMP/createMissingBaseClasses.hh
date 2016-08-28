#ifndef FGLUE_TMP_CREATE_MISSING_BASE_CLASSES_HH
#define FGLUE_TMP_CREATE_MISSING_BASE_CLASSES_HH

#include "and.hh"
#include "apply.hh"
#include "delay.hh"
#include "isBaseOf.hh"
#include "make.hh"
#include "or.hh"
#include "storeIf.hh"
#include "variadic.hh"

namespace FGlue
{
  template <class... Derived>
  using IsBaseOfOneOf = Apply< Variadic< Make<IsBaseOf> , Delay<Or> > , Derived... >;

  template <class... Derived>
  using IsNotBaseOfAnyOf = Apply< Make<IsNotBaseOf> , Delay<And> , Derived... >;


  template <class Operation, class... Derived>
  using EnableBaseClassesIf = Apply< Variadic<StoreIf<Operation>,Compose> , Derived...>;


  template <class... Derived>
  struct BaseOf
  {
    template <class... OtherBases>
    using NotBaseOf = StoreIf< Apply< Delay<And> , IsBaseOfOneOf<Derived...> , IsNotBaseOfAnyOf<OtherBases...> > >;
  };

  template <class... BaseClassCandidates>
  struct CreateMissingBases
  {
    template <class... Derived>
    struct BaseOf
    {
      template <class... OtherBases>
      using NotBaseOf = Apply< Variadic< typename FGlue::BaseOf<Derived...>::template NotBaseOf<OtherBases...> , Compose > , BaseClassCandidates... >;
    };
  };

//  template <class... BaseClassCandidates>
//  struct CreateMissingBases
//  {
//    template <class... Derived>
//    struct BaseOf
//    {
//      template <class... OtherBases>
//      using NotBaseOf = Apply< Variadic< FGlue::BaseOf2<Derived...>::NotBaseOf2<OtherBases...> , Compose > , BaseClassCandidates... >;
//    };
//  };
}

#endif // FGLUE_TMP_CREATE_MISSING_BASE_CLASSES_HH
