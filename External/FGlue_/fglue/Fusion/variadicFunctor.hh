// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FGLUE_FUSION_VARIADIC_FUNCTOR_HH
#define FGLUE_FUSION_VARIADIC_FUNCTOR_HH

#include <utility>
#include "fglue/TMP/constant.hh"
#include "fglue/TMP/trueFalse.hh"

namespace FGlue
{
  namespace Fusion
  {
    namespace VariadicFunctorDetail
    {
      template <class,class...> struct Apply;

      template <class Functor, class Arg, class... Args>
      struct Apply<Functor,Arg,Args...>
      {
        static void apply(Functor& f, Arg& arg, Args&... args)
        {
          f(arg);
          Apply<Functor,Args...>::apply(f,args...);
        }
      };

      template <class Functor, class Arg>
      struct Apply<Functor,Arg>
      {
        static void apply(Functor& f, Arg& arg)
        {
          f(arg);
        }
      };
    }

    template <class Functor>
    struct VariadicFunctor
    {
      template <class... Args>
      VariadicFunctor(Args&&... args)
        : f_(std::forward<Args>(args)...)
      {}

      template <class... Args>
      void operator()(Args&&... args)
      {
        VariadicFunctorDetail::Apply<Functor,Args...>::apply(f_,args...);
      }

      Functor f_;
    };
  }
}

#endif // FGLUE_FUSION_VARIADIC_FUNCTOR_HH

