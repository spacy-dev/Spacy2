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
    AbstractHilbertSpace();

    virtual ~AbstractHilbertSpace() = default;

    void setScalarProduct(std::shared_ptr<AbstractDualPairing> sp);

    std::shared_ptr<AbstractDualPairing> getScalarProduct() const;

  protected:
    std::shared_ptr<AbstractNorm> norm_;

  private:
    void setDualPairingImpl(std::shared_ptr<AbstractDualPairing> dp) final override;

    std::shared_ptr<AbstractDualPairing> getDualPairingImpl() const final override;

    virtual void setScalarProductImpl(std::shared_ptr<AbstractDualPairing>) = 0;

    virtual std::shared_ptr<AbstractDualPairing> getScalarProductImpl() const = 0;

    void setNormImpl(std::shared_ptr<AbstractNorm>) override;

    std::shared_ptr<AbstractNorm> getNormImpl() const override;
  };

  bool isHilbertSpace(const AbstractBanachSpace& space);
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
