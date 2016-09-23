#include "c1Operator.hh"

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Util/cast.hh>

#include "linearOperator.hh"
#include "linearOperatorCreator.hh"
#include "vector.hh"

#include <utility>

namespace Spacy
{
  namespace Rn
  {
    C1Operator::C1Operator(std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > value,
                           std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > derivative,
                           const VectorSpace& domain,
                           const VectorSpace& range) :
      OperatorBase(domain,range),
      value_(std::move(value)),
      derivative_(std::move(derivative)),
      operatorSpace_( std::make_shared<VectorSpace>(
//                        LinearOperatorCreator(domain,range) ,
                        [](const ::Spacy::VectorSpace*) { return Spacy::Real(0.); },
                        [](const ::Spacy::Vector&) { return Spacy::Real(0.); }
                      )
                    )
    {}

    ::Spacy::Vector C1Operator::operator()(const ::Spacy::Vector& x) const
    {
      return Vector( value_( cast_ref<Vector>(x).get() ) , range());
    }

    ::Spacy::Vector C1Operator::d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
    {
      return Vector( derivative_( cast_ref<Vector>(x).get() ) * cast_ref<Vector>(dx).get() , range() );
    }

    LinearOperator C1Operator::linearization(const ::Spacy::Vector& x) const
    {
      return LinearOperator(derivative_( cast_ref<Vector>(x).get()) , *operatorSpace_, domain(), range());
    }
  }
}
