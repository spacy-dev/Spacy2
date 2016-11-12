#pragma once

namespace Spacy
{
    class Real;
    class Vector;
    class VectorSpace;
}
namespace Mock
{
    class LinearOperator;

    struct C2Functional
    {
        C2Functional(const Spacy::VectorSpace& space);

        Spacy::Real operator()(const Spacy::Vector&) const;

        Spacy::Vector d1(const Spacy::Vector&) const;

        Spacy::Vector d2(const Spacy::Vector&, const Spacy::Vector&) const;

        LinearOperator hessian(const Spacy::Vector&) const;

        const Spacy::VectorSpace& domain() const;

    private:
        const Spacy::VectorSpace* domain_;
    };
}
