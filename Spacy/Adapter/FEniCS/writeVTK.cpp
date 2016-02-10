// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "writeVTK.hh"

#include "Spacy/vector.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Adapter/Generic/writeVTK.hh"
#include "vector.hh"

#include <dolfin.h>
#include <cassert>

namespace Spacy
{
  namespace FEniCS
  {
    void writeVTK(const Vector &x, const std::string& fileName)
    {
      dolfin::File file(fileName+".pvd");
      file << x.v_;
    }


    void writeVTK(const ::Spacy::Vector &x, const std::string& fileName)
    {
      void(*writer)(const Vector&,const std::string&) = &writeVTK;
      Generic::writeVTK<Vector>(x,fileName,writer);
    }
  }
}

