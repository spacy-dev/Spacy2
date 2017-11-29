#pragma once

#include <Spacy/vector.hh>

#include "vector.hh"

namespace Spacy
{
  namespace KaskadeParabolic
  {
    template <class Description>
    void writeVTK(const Vector<Description>& x, const char* fileName)
    {
      auto vc = ::Spacy::creator<VectorCreator<Description> >(x.space());
      auto gm = vc.getGridMan().getKaskGridMan();
      for(auto i = 0u ; i<gm.size() ; i++)
      {
        ::Kaskade::writeVTKFile(gm.at(i)->grid().leafGridView(), x.get(i) ,fileName+std::to_string(i),::Kaskade::IoOptions(),vc.getGridMan().getFEorder());
      }
    }

  }
}
