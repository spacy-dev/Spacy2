#ifndef ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH

#include "abstractBanachSpace.hh"

namespace Algorithm
{
  class AbstractScalarProduct;
  class AbstractNorm;
  class AbstractFunctionSpaceElement;

  class AbstractHilbertSpace : public AbstractBanachSpace
  {
  public:
    AbstractHilbertSpace();

    virtual ~AbstractHilbertSpace() = default;

    void setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp);

    std::shared_ptr<AbstractScalarProduct> getScalarProduct() const;

  protected:
    std::shared_ptr<AbstractNorm> norm_;

  private:
    virtual void setScalarProductImpl(std::shared_ptr<AbstractScalarProduct>) = 0;

    virtual std::shared_ptr<AbstractScalarProduct> getScalarProductImpl() const = 0;

    void setNormImpl(std::shared_ptr<AbstractNorm>) override;

    std::shared_ptr<AbstractNorm> getNormImpl() const override;
  };

  double operator* (const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&);
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
