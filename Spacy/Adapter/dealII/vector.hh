#pragma once

#include <deal.II/lac/vector.h>
#include <Spacy/Adapter/Generic/vector.hh>

namespace Spacy
{
    namespace dealII
    {
        /**
         * @ingroup dealIIGroup, VectorSpaceGroup
         * @brief %Vector adapter for deal.II
         */
        using Vector = Generic::Vector< dealii::Vector<double> >;
    }
}
