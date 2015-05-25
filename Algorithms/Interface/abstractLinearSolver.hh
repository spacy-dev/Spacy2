#ifndef ALGORITHM_INTERFACE_ABSTRACTLINEARSOLVER_HH
#define ALGORITHM_INTERFACE_ABSTRACTLINEARSOLVER_HH

#include <memory>
#include <utility>

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractLinearSolver
  {
  public:
    virtual ~AbstractLinearSolver(){}

    virtual FunctionSpaceElement operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const = 0;

    virtual FunctionSpaceElement operator()(const FunctionSpaceElement& y) const = 0;
  };


}

#endif // ALGORITHM_INTERFACE_ABSTRACTLINEARSOLVER_HH
