#include "linearOperator.hh"

#include "linearSolver.hh"
#include "vector.hh"

namespace Spacy
{
  namespace Rn
  {
    LinearOperator::LinearOperator(::Eigen::MatrixXd A, const VectorSpace& space, const VectorSpace& domain, const VectorSpace& range)
        : Mixin::Impl< ::Eigen::MatrixXd >(std::move(A)),
          VectorBase(space),
          OperatorBase(domain,range)
      {}

    ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
    {
      return Vector( impl()*cast_ref<Vector>(dx).impl(), dx.space() );
    }

    ::Spacy::LinearSolver LinearOperator::solver() const
    {
      return LinearSolver(impl(),domain());
    }
  }
}
