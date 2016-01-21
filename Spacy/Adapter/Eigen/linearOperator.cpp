#include "linearOperator.hh"
#include "util.hh"
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
// This copying is necessary to cover the case, where the Spacy Vector consists of serveral Eigen vectors		
	 ::Eigen::VectorXd dx_;
     copy(dx,dx_);           
     ::Eigen::VectorXd x_=get()*dx_;
     ::Spacy::Vector x=dx.space().zeroVector();
     copy(x_,x);
     return x;	
  //    return Vector( get()*cast_ref<Vector>(dx).get(), dx.space() );
    }

    LinearSolver LinearOperator::solver() const
    {
      return LinearSolver(get(),domain());
    }
  }
}
