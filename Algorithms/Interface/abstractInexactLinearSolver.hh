#ifndef ALGORITHM_INTERFACE_ABSTRACTINEXACTLINEARSOLVER_HH
#define ALGORITHM_INTERFACE_ABSTRACTINEXACTLINEARSOLVER_HH

namespace Algorithm
{
  class AbstractInexactLinearSolver
  {
  public:
    virtual void setTolerance() = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACTINEXACTLINEARSOLVER_HH
