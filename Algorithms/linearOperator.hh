#ifndef ALGORITHM_LINEAR_OPERATOR_HH
#define ALGORITHM_LINEAR_OPERATOR_HH

#include <memory>

#include "vector.hh"
#include "linearSolver.hh"
#include "operator.hh"
#include "Interface/Operator/abstractLinearOperator.hh"

namespace Algorithm
{
  /// \cond
  class LinearSolver;
  /// \endcond

  class LinearOperator : public Operator
  {
  public:
    LinearOperator(std::unique_ptr<Interface::AbstractLinearOperator>&& impl);

//    LinearOperator(const LinearOperator&);

//    LinearOperator& operator=(const LinearOperator&);

//    Vector operator()(const Vector&) const;

    LinearSolver solver() const;

    LinearSolver adjointSolver() const;
  };

  LinearSolver operator^(const LinearOperator& A, int k);

//  class LinearizedOperator
//  {
//  public:
//    LinearizedOperator(std::shared_ptr<AbstractLinearizedOperator> impl);

//    Vector operator()(const Vector&) const;

//    void setArgument(const Vector& x);

//    LinearSolver getSolver() const;

//    AbstractLinearizedOperator& impl();

//    const AbstractLinearizedOperator& impl() const;

//  private:
//    std::shared_ptr<AbstractLinearizedOperator> impl_;
//  };
}

#endif // ALGORITHM_LINEAR_OPERATOR_HH
