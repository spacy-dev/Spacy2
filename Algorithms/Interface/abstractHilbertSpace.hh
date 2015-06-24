#ifndef ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH

#include "abstractBanachSpace.hh"
#include "abstractScalarProduct.hh"

namespace Algorithm
{
  class AbstractNorm;
  class AbstractFunctionSpaceElement;

  class AbstractHilbertSpace : public AbstractBanachSpace
  {
  public:
    AbstractHilbertSpace(std::shared_ptr<AbstractScalarProduct> sp);

    virtual ~AbstractHilbertSpace() = default;

    void setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp);

    std::shared_ptr<AbstractScalarProduct> getScalarProduct() const;

  private:
    std::shared_ptr<AbstractScalarProduct> sp_;
  };

  bool isHilbertSpace(const AbstractBanachSpace& space);

  double operator* (const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&);
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_HILBERT_SPACE_HH
