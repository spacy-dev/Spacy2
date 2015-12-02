#include "c1Operator.hh"

#include "Spacy/Spaces/realSpace.hh"
#include "Spacy/vectorSpace.hh"
#include "linearOperator.hh"
#include "linearOperatorCreator.hh"

#include <cassert>

namespace Spacy
{
  namespace Scalar
  {
    C1Operator::C1Operator(std::function<double(double)> value,
                           std::function<double(double)> derivative)
      : OperatorBase(Space::R,Space::R),
        value_(value),
        derivative_(derivative),
        operatorSpace_( std::make_shared<VectorSpace>(
                          LinearOperatorCreator() ,
                          [](const ::Spacy::Vector& v)
                          { return Real(0.); } ,
                          true
                      ) )
    {}

    Spacy::Vector C1Operator::operator()(const ::Spacy::Vector& x) const
    {
      assert( value_ );
      return Real( value_( toDouble(x) ) );
    }

    Spacy::Vector C1Operator::d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
    {
      assert(derivative_);
      return Real( derivative_( toDouble(x) ) * toDouble(dx) );
    }

    LinearOperator C1Operator::linearization(const ::Spacy::Vector& x) const
    {
      assert(derivative_);
      assert(operatorSpace_ != nullptr);
      return LinearOperator( *operatorSpace_, derivative_( toDouble(x) ) );
    }
  }
}
