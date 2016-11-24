#include "c2Functional.hh"

#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/cast.hh>

#include "linearOperator.hh"

namespace Spacy
{
    namespace Scalar
    {
        C2Functional::C2Functional(std::function<double(double)> value,
                                   std::function<double(double)> derivative,
                                   std::function<double(double)> secDerivative) :
            Spacy::FunctionalBase(Space::R) ,
            value_(std::move(value)),
            derivative_(std::move(derivative)),
            secDerivative_(std::move(secDerivative))
        {}

        Spacy::Real C2Functional::operator()(const ::Spacy::Vector& x) const
        {
            return value_(get(cast_ref<Spacy::Real>(x)));
        }

        Spacy::Vector C2Functional::d1(const ::Spacy::Vector& x) const
        {
            return Spacy::Real(derivative_(get(cast_ref<Spacy::Real>(x))));
        }

        Spacy::Vector C2Functional::d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
        {
            return Spacy::Real( secDerivative_( get(cast_ref<Spacy::Real>(x))) * get(cast_ref<Spacy::Real>(dx)) );
        }
        Spacy::Scalar::LinearOperator C2Functional::hessian(const ::Spacy::Vector& x) const
        {
            return Spacy::Scalar::LinearOperator( *operatorSpace_, secDerivative_( get(cast_ref<Spacy::Real>(x)) ));
        }
    }
}
