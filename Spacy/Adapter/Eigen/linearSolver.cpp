#include "linearSolver.hh"

#include <Spacy/Util/cast.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include "vector.hh"
#include "util.hh"
#include <utility>

namespace Spacy
{
  namespace Rn
  {
    LinearSolver::LinearSolver(::Eigen::MatrixXd A,const VectorSpace& domain)
      : A_(std::move(A)), domain_(domain) {}

    Spacy::Vector LinearSolver::operator()(const ::Spacy::Vector& y) const
    {
		  ::Eigen::VectorXd y_;
         copy(y,y_);     
         ::Eigen::VectorXd x_ = A_.lu().solve(y_);
         ::Spacy::Vector x = zero( domain_ );
         copy(x_,x);
        return x;
    }
  }
}

