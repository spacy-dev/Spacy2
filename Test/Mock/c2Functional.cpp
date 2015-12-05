// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "c2Functional.hh"

namespace Mock
{
  C2Functional::C2Functional(const Spacy::VectorSpace& space)
    : domain_(&space)
  {}

  Spacy::Real C2Functional::operator()(const Spacy::Vector&) const
  {
    return Spacy::Real(3.);
  }

  Spacy::Vector C2Functional::d1(const Spacy::Vector&) const
  {
    return Spacy::Real(2.);
  }

  Spacy::Vector C2Functional::d2(const Spacy::Vector&, const Spacy::Vector&) const
  {
    return Spacy::Real(1.);
  }

  LinearOperator C2Functional::hessian(const Spacy::Vector&) const
  {
    return LinearOperator();
  }

  const Spacy::VectorSpace& C2Functional::domain() const
  {
    return *domain_;
  }
}
