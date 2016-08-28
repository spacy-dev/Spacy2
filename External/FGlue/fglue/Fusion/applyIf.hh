// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FGLUE_FUSION_APPLY_IF_HH
#define FGLUE_FUSION_APPLY_IF_HH

#include <utility>

#include "fglue/TMP/apply.hh"
#include "fglue/TMP/trueFalse.hh"

namespace FGlue
{
  namespace Fusion
  {
    template <class Operation, class CompileTimeDecider>
    struct UnaryApplyIf
    {
      template <class... Args>
      UnaryApplyIf(Args&&... args)
        : f_(std::forward<Args>(args)...)
      {}

      template <class Arg,
                std::enable_if_t< !isTrue< Apply< CompileTimeDecider , std::decay_t<Arg> > >() >* = nullptr >
      auto operator()(Arg&& arg)
      {}

      template <class Arg,
                std::enable_if_t< isTrue< Apply< CompileTimeDecider , std::decay_t<Arg> > >() >* = nullptr >
      auto operator()(Arg&& arg)
      {
        f_(arg);
      }

    private:
      Operation f_;
    };

  }
}

#endif // FGLUE_FUSION_APPLY_IF_HH

