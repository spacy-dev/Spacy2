#pragma once

#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>

#include <Spacy/vector.hh>

namespace Spacy
{
    /** @addtogroup dealIIGroup @{ */
    namespace dealII
    {
        void copy(const Spacy::Vector& from, dealii::Vector<double>& to);

        void copy(const dealii::Vector<double>& from, Spacy::Vector& to);

        void copy(const Spacy::Vector& from, dealii::BlockVector<double>& to);

        void copy(const dealii::BlockVector<double>& from, Spacy::Vector& to);
    }
    /** @} */
}
