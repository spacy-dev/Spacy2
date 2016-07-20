// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "trivialPreconditioner.hh"

#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/vectorSpace.hh"

#include <utility>

namespace Spacy
{
  namespace CG
  {
    TrivialPreconditioner::TrivialPreconditioner( const VectorSpace& domain,
                                                  const VectorSpace& range)
      : OperatorBase(domain,range)
    {}

    Vector TrivialPreconditioner::operator()(const Vector& x) const
    {
      return x;
    }

  }
}

