// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_TO_VTK_HH
#define SPACY_ADAPTER_KASKADE_TO_VTK_HH

#include <string>
#include "vector.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  namespace Kaskade
  {
    template <class Description>
    void writeVTK(const Vector<Description>& x, const std::string& fileName)
    {
      typename Description::VariableSet y(*x.description_);
      copy(x,y);
      ::Kaskade::writeVTKFile(boost::fusion::at_c<0>(x.description_->spaces)->gridManager().grid().leafGridView(),
                              y, fileName, ::Kaskade::IoOptions{}, 1);//at_c<0>(spaces_)->mapper().getOrder());
    }

//    template <class Description>
//    void writeVTK(const ::Spacy::Vector& x, const std::string& fileName)
//    {
//      void(*writer)(const Vector&,const std::string&) = &writeVTK;
//      Generic::writeVTK<Vector>(x,fileName,writer);
//    }
  }
}

#endif // SPACY_ADAPTER_KASKADE_TO_VTK_HH
