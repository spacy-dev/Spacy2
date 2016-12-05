#include "copy.hh"

#include <Spacy/Adapter/dealII/vector.hh>
#include <Spacy/Util/cast.hh>

namespace Spacy
{
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
    }
}
