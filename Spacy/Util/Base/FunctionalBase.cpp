#include "FunctionalBase.hh"

#include <Spacy/vectorSpace.hh>

namespace Spacy
{
    FunctionalBase::FunctionalBase(const VectorSpace& domain)
        : domain_(domain)
    {}

    const VectorSpace& FunctionalBase::domain() const
    {
        return domain_;
    }
}
