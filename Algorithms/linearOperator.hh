#ifndef ALGORITHM_LINEAR_OPERATOR_HH
#define ALGORITHM_LINEAR_OPERATOR_HH

#include <memory>

#include "functionSpaceElement.hh"
#include "linearSolver.hh"
#include "operator.hh"
#include "Interface/Operator/abstractLinearOperator.hh"

namespace Algorithm
{
  class LinearizedOperator;

  class LinearOperator : public Operator
  {
  public:
    LinearOperator(const AbstractLinearOperator& impl);

    LinearOperator(const LinearizedOperator& impl);

    LinearOperator(const LinearOperator&);

    LinearOperator& operator=(const LinearOperator&);

    FunctionSpaceElement operator()(const FunctionSpaceElement&) const;

    LinearSolver getSolver() const;

    AbstractLinearOperator& impl();

    const AbstractLinearOperator& impl() const;

  private:
    std::unique_ptr<AbstractLinearOperator> impl_;
  };

  LinearSolver operator^(const LinearOperator& A, int k);

//  class LinearizedOperator
//  {
//  public:
//    LinearizedOperator(std::shared_ptr<AbstractLinearizedOperator> impl);

//    FunctionSpaceElement operator()(const FunctionSpaceElement&) const;

//    void setArgument(const FunctionSpaceElement& x);

//    LinearSolver getSolver() const;

//    AbstractLinearizedOperator& impl();

//    const AbstractLinearizedOperator& impl() const;

//  private:
//    std::shared_ptr<AbstractLinearizedOperator> impl_;
//  };
}

#endif // ALGORITHM_LINEAR_OPERATOR_HH
