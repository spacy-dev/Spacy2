#pragma once

#include <Spacy/linearSolver.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/Util/Base/AddArithmeticOperators.hh>
#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/Util/Base/VectorBase.hh>

namespace Mock
{
  class LinearOperator
      : public Spacy::Mixin::Get<double>,
        public Spacy::OperatorBase,
        public Spacy::VectorBase,
        public Spacy::AddArithmeticOperators<LinearOperator>
  {
  public:
    LinearOperator(double value=1);

    ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

    ::Spacy::Real operator()(const LinearOperator& y) const;

    /// Access solver representing \f$A^{-1}\f$
    ::Spacy::LinearSolver solver() const;
  };
}
