#ifndef ALGORITHM_INTERFACE_ABSTRACT_NORM_HH
#define ALGORITHM_INTERFACE_ABSTRACT_NORM_HH

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;

    class AbstractNorm
    {
    public:
      virtual ~AbstractNorm(){}

      virtual double operator()(const AbstractFunctionSpaceElement&) const = 0;

      virtual double squared(const AbstractFunctionSpaceElement&) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_NORM_HH
