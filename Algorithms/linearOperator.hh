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

    FunctionSpaceElement operator()(const FunctionSpaceElement&) const;

    LinearSolver getSolver() const;

//    AbstractLinearOperator& impl() override;

    const AbstractLinearOperator& impl() const;

  private:
    const AbstractLinearOperator& impl_;
  };

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
