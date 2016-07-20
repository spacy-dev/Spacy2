// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "directPreconditioner.hh"

#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/vectorSpace.hh"

#include <utility>
#include <iostream>

namespace Spacy
{
  namespace CG
  {
    DirectPreconditioner::DirectPreconditioner(   ::Spacy::C2Functional M,  
                                                  const ::Spacy::Vector& origin,  
                                                  const VectorSpace& domain,
                                                  const VectorSpace& range)
      : OperatorBase(domain,range),
        M_(M),
        origin_(origin),
        directSolver(new ::Spacy::LinearSolver(M_.hessian(origin_).solver()))
    {
		std::cout << "Creating direct solver: " << directSolver.get() << std::endl;
	}

    Vector DirectPreconditioner::operator()(const Vector& x) const
    {
      return (*directSolver)(x);
    }

  }
}

