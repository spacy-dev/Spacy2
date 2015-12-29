#include "c2Functional.hh"


#include "linearOperator.hh"
#include "linearOperatorCreator.hh"
#include "vector.hh"
#include "scalarProduct.hh"
#include "vectorCreator.hh"

#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Rn
  {
    C2Functional::C2Functional(std::function< double(const Eigen::VectorXd&) > f,
                               std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df,
                               std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf,
                               const VectorSpace& domain)
      : Spacy::FunctionalBase(domain),
        f_(f),
        df_(df),
        ddf_(ddf),
        operatorSpace_( std::make_shared<VectorSpace>(
                          LinearOperatorCreator(domain,domain.dualSpace()) ,
                          [](const ::Spacy::Vector& v) { return Spacy::Real(0.); }
                        )
                      )
      {}

    Spacy::Real C2Functional::operator()(const ::Spacy::Vector& x) const
    {
      return f_(cast_ref<Vector>(x).get());
    }

    Spacy::Vector C2Functional::d1(const ::Spacy::Vector& x) const
    {
      return Vector(df_(cast_ref<Vector>(x).get()),domain());
    }

    Spacy::Vector C2Functional::d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
    {
      return hessian(x)(dx);
    }

    LinearOperator C2Functional::hessian(const ::Spacy::Vector& x) const
    {
      return LinearOperator( ddf_(cast_ref<Vector>(x).get() ), *operatorSpace_, domain(), domain().dualSpace());
    }
  }
}
