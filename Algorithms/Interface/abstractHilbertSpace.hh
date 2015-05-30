#ifndef ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH

#include "abstractBanachSpace.hh"

namespace Algorithm
{
  class AbstractDualPairing;
  class AbstractNorm;
  class AbstractFunctionSpaceElement;

  class AbstractHilbertSpace : public AbstractBanachSpace
  {
  public:
    AbstractHilbertSpace(std::shared_ptr<AbstractDualPairing> sp);

    virtual ~AbstractHilbertSpace() = default;

    void setScalarProduct(std::shared_ptr<AbstractDualPairing> sp);

    std::shared_ptr<AbstractDualPairing> getScalarProduct() const;
  };

  bool isHilbertSpace(const AbstractBanachSpace& space);
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
