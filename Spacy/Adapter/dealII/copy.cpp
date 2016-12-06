#include "copy.hh"

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>

#include "vector.hh"

namespace Spacy
{
    namespace
    {
        void copyTodealIIVectorIfConsistent(const Vector& from, dealii::BlockVector<double>& to, unsigned i)
        {
            if( !is<dealII::Vector>(from) ) return;
            to.block(i) = get(cast_ref<dealII::Vector>(from));
        }

        void copyFromdealIIVectorIfConsistent(const dealii::BlockVector<double>& from, Vector& to, unsigned i)
        {
            if( !is<dealII::Vector>(to) ) return;
            get(cast_ref<dealII::Vector>(to)) = from.block(i);
        }
    }

    namespace dealII
    {
        void copy(const Spacy::Vector& from, dealii::Vector<double>& to)
        {
            to = get(cast_ref<Vector>(from));
        }

        void copy(const dealii::Vector<double>& from, Spacy::Vector& to)
        {
            get(cast_ref<Vector>(to)) = from;
        }

        void copy(const Spacy::Vector& from, dealii::BlockVector<double>& to, unsigned i)
        {
            copyTodealIIVectorIfConsistent(from, to, i);

            if(is<ProductSpace::Vector>(from))
            {
                auto& from_ = cast_ref<ProductSpace::Vector>(from);

                for(auto j=0u; j<from_.numberOfVariables(); ++j)
                    copy(from_.component(j), to, creator<ProductSpace::VectorCreator>(from_.space()).inverseIdMap(j));
            }
        }

        void copy(const dealii::BlockVector<double>& from, Spacy::Vector& to, unsigned i)
        {
            copyFromdealIIVectorIfConsistent(from, to, i);

            if(is<ProductSpace::Vector>(to))
            {
                auto& to_ = cast_ref<ProductSpace::Vector>(to);

                for(auto j=0u; j<to_.numberOfVariables(); ++j)
                    copy(from, to_.component(j), creator<ProductSpace::VectorCreator>(to_.space()).inverseIdMap(j));
            }
        }
    }
}
