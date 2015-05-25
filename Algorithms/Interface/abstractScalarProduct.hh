#ifndef ALGORITHM_INTERFACE_ABSTRACT_SCALARPRODUCT_HH
#define ALGORITHM_INTERFACE_ABSTRACT_SCALARPRODUCT_HH

namespace Algorithm
{
  class AbstractFunctionSpaceElement;

  class AbstractScalarProduct
  {
  public:
    virtual ~AbstractScalarProduct(){}

    virtual double operator()(const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_SCALARPRODUCT_HH
