#include "linearOperator.hh"

#include "linearSolver.hh"
#include "util.hh"
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
      ::Eigen::VectorXd dx_;
      copy(dx,dx_);
      auto x_ = get() * dx_;
      auto x = dx.space().zeroVector();
      copy(x_,x);
      return x;
    }

    LinearSolver LinearOperator::solver() const
    {
      return LinearSolver(get(),domain());
    }
  }
}
