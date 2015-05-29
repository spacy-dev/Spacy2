#ifndef ALGORITHM_INTERFACE_ABSTRACT_DUAL_PAIRING_HH
#define ALGORITHM_INTERFACE_ABSTRACT_DUAL_PAIRING_HH

namespace Algorithm
{
  class AbstractFunctionSpaceElement;

  class AbstractDualPairing
  {
  public:
    virtual ~AbstractDualPairing(){}

    virtual double operator()(const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_DUAL_PAIRING_HH
