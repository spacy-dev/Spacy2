// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FGLUE_FUSION_CONNECT_HH
#define FGLUE_FUSION_CONNECT_HH

#include "fglue/TMP/apply.hh"
#include "fglue/TMP/trueFalse.hh"
#include "fglue/Fusion/applyIf.hh"
#include "fglue/Fusion/castAndAttach.hh"
#include "fglue/Fusion/variadicFunctor.hh"

namespace FGlue
{
  template <class SourceOperation, class TargetOperation = SourceOperation>
  struct Connector
  {
    template <class Base, class Source, bool valid = isTrue< Apply<SourceOperation,Source> >() >
    struct From
    {
      From(Source& source)
        : castAndAttach(source)
      {}

      template <class... Targets>
      void to(Targets&... targets)
      {
        castAndAttach(targets...);
      }

      Fusion::VariadicFunctor< Fusion::UnaryApplyIf< Fusion::CastAndAttach<Base> , TargetOperation> > castAndAttach;
    };

    template <class Base, class Source>
    struct From<Base,Source,false>
    {
      template <class Arg>
      From(Arg&){}

      template <class... Targets>
      void to(Targets&...)
      {}
    };

    template <class Base, class Source>
    static auto from(Source& source)
    {
      return From<Base,Source>(source);
    }
  };

  template <class SourceOperation, class TargetOperation = SourceOperation>
  struct Deconnector
  {
    template <class Base, class Source, bool valid = isTrue< Apply<SourceOperation,Source> >() >
    struct From
    {
      From(Source& source)
        : castAndDetach(source)
      {}

      template <class... Targets>
      void to(Targets&... targets)
      {
        castAndDetach(targets...);
      }

      Fusion::VariadicFunctor< Fusion::UnaryApplyIf< Fusion::CastAndDetach<Base> , TargetOperation> > castAndDetach;
    };

    template <class Base, class Source>
    struct From<Base,Source,false>
    {
      template <class Arg>
      From(Arg&){}

      template <class... Targets>
      void to(Targets&...)
      {}
    };

    template <class Base, class Source>
    static auto from(Source& source)
    {
      return From<Base,Source>(source);
    }
  };
}

#endif // FGLUE_FUSION_CONNECT_HH
