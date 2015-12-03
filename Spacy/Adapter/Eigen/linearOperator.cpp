#include "linearOperator.hh"

#include "linearSolver.hh"
#include "vector.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  namespace Rn
  {
    LinearOperator::LinearOperator(::Eigen::MatrixXd A, const VectorSpace& space, const VectorSpace& domain, const VectorSpace& range)
        : Mixin::Get< ::Eigen::MatrixXd >(std::move(A)),
          VectorBase(space),
          OperatorBase(domain,range)
      {}

    ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
    {
      return Vector( get()*cast_ref<Vector>(dx).get(), dx.space() );
    }

    LinearSolver LinearOperator::solver() const
    {
      return LinearSolver(get(),domain());
    }
  }
}
