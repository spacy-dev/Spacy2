#ifndef ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
#define ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH

#include <memory>
#include "abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractScalarProduct;
    class AbstractNorm;

    static unsigned spaceIndex = 0;

    class AbstractBanachSpace
    {
    public:
      AbstractBanachSpace(std::shared_ptr<AbstractNorm> norm);

      virtual ~AbstractBanachSpace() = default;

      void setNorm(std::shared_ptr<AbstractNorm> norm);

      const AbstractNorm& getNorm() const;

      std::shared_ptr<AbstractNorm> getSharedNorm() const;

      std::unique_ptr<AbstractFunctionSpaceElement> element() const;

      unsigned index() const;

      void addPrimalSpace(const AbstractBanachSpace& primalSpace);

      void addDualSpace(const AbstractBanachSpace& dualSpace);

      bool isPrimalWRT(const AbstractBanachSpace& dualSpace) const;

      bool isDualWRT(const AbstractBanachSpace& primalSpace) const;

    protected:
      std::shared_ptr<AbstractNorm> norm_ = nullptr;
      virtual std::unique_ptr<AbstractFunctionSpaceElement> elementImpl() const = 0;

    private:
      std::vector<unsigned> primalSpaces_, dualSpaces_;

      const unsigned index_ = spaceIndex++;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_BANACH_SPACE_HH
