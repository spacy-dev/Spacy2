// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "util.hh"
#include "Eigen/Dense"
#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/Adapter/Eigen/vector.hh"
#include "Spacy/Adapter/Generic/vector.hh"

namespace Spacy
{
  namespace Rn
  {
    namespace
    {
      void copyToFlatVector(const ::Spacy::Vector& x, Eigen::VectorXd& y, unsigned& currentIndex)
      {
        if( is<Vector>(x) )
        {
          y.block(currentIndex,0,cast_ref<Vector>(x).get().size(),1)=cast_ref<Vector>(x).get();
          currentIndex += cast_ref<Vector>(x).get().size();
          return;
        }

        if( is<ProductSpace::Vector>(x))
        {
          for(auto i=0u; i < cast_ref<ProductSpace::Vector>(x).numberOfVariables(); ++i)
            copyToFlatVector(cast_ref<ProductSpace::Vector>(x).component(i),y,currentIndex);
          return;
        }
      }

      void copyFromFlatVector(const Eigen::VectorXd& x,::Spacy::Vector& y, unsigned& currentIndex)
      {
        if( is<Vector>(y) )
        {
          cast_ref<Vector>(y).get()=x.block(currentIndex,0,cast_ref<Vector>(y).get().size(),1);
          currentIndex += cast_ref<Vector>(y).get().size();
          return;
        }

        if( is<ProductSpace::Vector>(y))
        {
          for(unsigned int i=0; i < cast_ref<ProductSpace::Vector>(y).numberOfVariables(); ++i)
            copyFromFlatVector(x,cast_ref<ProductSpace::Vector>(y).component(i),currentIndex);
          return;
        }
      }

      void getSizeOfFlatVector(const ::Spacy::Vector& y, unsigned& currentIndex)
      {
        if( is<Vector>(y) )
        {
          currentIndex += cast_ref<Vector>(y).get().size();
          return;
        }

        if( is<ProductSpace::Vector>(y))
        {
          for(unsigned int i=0; i < cast_ref<ProductSpace::Vector>(y).numberOfVariables(); ++i)
            getSizeOfFlatVector(cast_ref<ProductSpace::Vector>(y).component(i),currentIndex);
          return;
        }
      }
    }


    void copy(const ::Spacy::Vector& x, Eigen::VectorXd& y)
    {
      auto index=0u;
      y.resize(getSize(x));
      copyToFlatVector(x,y,index);
    }

    void copy(const Eigen::VectorXd& x,::Spacy::Vector& y)
    {
      auto index=0u;
      copyFromFlatVector(x,y,index);
    }

    unsigned getSize(const ::Spacy::Vector& y)
    {
      auto index=0u;
      getSizeOfFlatVector(y,index);
      return index;
    }
  }
}
