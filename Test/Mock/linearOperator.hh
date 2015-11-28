// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_TEST_MOCK_LINEAR_OPERATOR_HH
#define SPACY_TEST_MOCK_LINEAR_OPERATOR_HH

#include "Spacy/linearSolver.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Base/vectorBase.hh"

namespace Mock
{
  class LinearOperator
      : public Spacy::Mixin::Impl<double>,
        public Spacy::OperatorBase,
        public Spacy::VectorBase,
        public Spacy::AddArithmeticOperators<LinearOperator>
  {
  public:
    LinearOperator(double value=1);

    ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

    /// Access solver representing \f$A^{-1}\f$
    ::Spacy::LinearSolver solver() const;
  };
}

#endif // SPACY_TEST_MOCK_LINEAR_OPERATOR_HH
