#ifndef ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH

#include <memory>

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;

    class AbstractLinearSolver
    {
    public:
      ~AbstractLinearSolver(){}

      virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
