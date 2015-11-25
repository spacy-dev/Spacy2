#include "linearSolver.hh"

#include "vector.hh"
#include "Spacy/Util/cast.hh"

namespace Spacy
{
  namespace Rn
  {
    LinearSolver::LinearSolver(::Eigen::MatrixXd A,const VectorSpace& domain)
      : A_(std::move(A)), domain_(domain) {}

    Spacy::Vector LinearSolver::operator()(const ::Spacy::Vector& y) const
    {
      return Vector(A_.lu().solve(cast_ref<Vector>(y).impl() ) , domain_ );
    }
  }
}

