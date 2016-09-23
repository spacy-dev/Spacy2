#include "util.hh"

#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Adapter/Eigen/vector.hh>
#include <Spacy/Adapter/Generic/vector.hh>

namespace Spacy
{
    namespace Rn
    {
        /**
     * @ingroup EigenGroup, VectorSpaceGroup
     * @brief %Vector for %Rn, based on the %Eigen library.
     */
        ///  Copy ::Spacy::Vector to flat coefficient vector of %Eigen .
        void copyToFlatVector(const ::Spacy::Vector& x, Eigen::VectorXd& y, unsigned& currentIndex)
        {
            if( is<Vector>(x) )
            {
                y.block(currentIndex,0,cast_ref<Vector>(x).get().size(),1)=cast_ref<Vector>(x).get();
                currentIndex += cast_ref<Vector>(x).get().size();
                return;
            }

            if(is<::Spacy::ProductSpace::Vector>(x))
            {
                for(unsigned int i=0; i < cast_ref<::Spacy::ProductSpace::Vector>(x).numberOfVariables(); ++i)
                {
                    copyToFlatVector(cast_ref< ::Spacy::ProductSpace::Vector>(x).component(i),y,currentIndex);
                }
                return;
            }
        }

        void copy(const ::Spacy::Vector& x, Eigen::VectorXd& y)
        {
            unsigned index=0;
            y.resize(getSize(x));
            copyToFlatVector(x,y,index);
        }


        ///  Copy flat coefficient vector of %Eigen to ::Spacy::Vector.
        void copyFromFlatVector(const Eigen::VectorXd& x,::Spacy::Vector& y, unsigned& currentIndex)
        {
            if( is<Vector>(y) )
            {
                cast_ref<Vector>(y).get()=x.block(currentIndex,0,cast_ref<Vector>(y).get().size(),1);
                currentIndex += cast_ref<Vector>(y).get().size();
                return;
            }

            if( is<::Spacy::ProductSpace::Vector>(y))
            {
                for(unsigned int i=0; i < cast_ref<::Spacy::ProductSpace::Vector>(y).numberOfVariables(); ++i)
                {
                    copyFromFlatVector(x,cast_ref<::Spacy::ProductSpace::Vector>(y).component(i),currentIndex);
                }
                return;
            }
        }

        void copy(const Eigen::VectorXd& x,::Spacy::Vector& y)
        {
            unsigned index=0;
            copyFromFlatVector(x,y,index);
        }

        void getSizeOfFlatVector(const ::Spacy::Vector& y, unsigned& currentIndex)
        {
            if( is<Vector>(y) )
            {
                currentIndex += cast_ref<Vector>(y).get().size();
                return;
            }

            if( is<::Spacy::ProductSpace::Vector>(y))
            {
                for(unsigned int i=0; i < cast_ref<::Spacy::ProductSpace::Vector>(y).numberOfVariables(); ++i)
                {
                    getSizeOfFlatVector(cast_ref<::Spacy::ProductSpace::Vector>(y).component(i),currentIndex);
                }
                return;
            }
        }

        unsigned getSize(const ::Spacy::Vector& y)
        {
            unsigned index=0;
            getSizeOfFlatVector(y,index);
            return index;
        }
    }
}
