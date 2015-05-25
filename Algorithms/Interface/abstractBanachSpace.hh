#ifndef ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH

#include <memory>
#include "abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  class AbstractNorm;

  static unsigned spaceIndex = 0;

  class AbstractBanachSpace
  {
  public:
    virtual ~AbstractBanachSpace() = default;

    void setNorm(std::shared_ptr<AbstractNorm> norm)
    {
      setNormImpl(norm);
    }

    std::shared_ptr<AbstractNorm> getNorm() const;

    std::unique_ptr<AbstractFunctionSpaceElement> element() const;

    unsigned index() const;

  private:
    virtual void setNormImpl(std::shared_ptr<AbstractNorm>) = 0;

    virtual std::shared_ptr<AbstractNorm> getNormImpl() const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const = 0;

    const unsigned index_ = spaceIndex++;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
