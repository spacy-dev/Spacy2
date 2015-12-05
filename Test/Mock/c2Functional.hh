// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_TEST_MOCK_C2_FUNCTIONAL_HH
#define SPACY_TEST_MOCK_C2_FUNCTIONAL_HH

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "linearOperator.hh"

namespace Mock
{
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

#endif // SPACY_TEST_MOCK_C2_FUNCTIONAL_HH
