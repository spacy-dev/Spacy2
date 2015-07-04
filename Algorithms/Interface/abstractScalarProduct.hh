#ifndef ALGORITHM_INTERFACE_ABSTRACT_SCALAR_PRODUCT_HH
#define ALGORITHM_INTERFACE_ABSTRACT_SCALAR_PRODUCT_HH

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;

    class AbstractScalarProduct
    {
    public:
      virtual ~AbstractScalarProduct(){}

      virtual double operator()(const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_SCALAR_PRODUCT_HH
