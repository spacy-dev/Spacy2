#ifndef ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH

#include <memory>
#include "abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  class AbstractDualPairing;
  class AbstractNorm;

  static unsigned spaceIndex = 0;

  class AbstractBanachSpace
  {
  public:
    AbstractBanachSpace(const AbstractBanachSpace* dualSpace, std::shared_ptr<AbstractNorm> norm);

    virtual ~AbstractBanachSpace() = default;

    void setNorm(std::shared_ptr<AbstractNorm> norm);

    std::shared_ptr<AbstractNorm> getNorm() const;

    void setDualPairing(std::shared_ptr<AbstractDualPairing> dp);

    std::shared_ptr<AbstractDualPairing> getDualPairing() const;

    std::unique_ptr<AbstractFunctionSpaceElement> element() const;

    unsigned index() const;

    void setDualSpace(AbstractBanachSpace& dualSpace);

    const AbstractBanachSpace& getDualSpace() const;

    bool hasDualSpace() const;

  protected:
    const AbstractBanachSpace* dualSpace_ = nullptr;
    std::shared_ptr<AbstractNorm> norm_ = nullptr;
    std::shared_ptr<AbstractDualPairing> dp_;

  private:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const = 0;

    const unsigned index_ = spaceIndex++;
};

  double operator* (const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&);
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
