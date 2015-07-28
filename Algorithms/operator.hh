#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "banachSpace.hh"
#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class Operator : public Mixin::UniqueImpl<Interface::AbstractOperator>
  {
  public:
    Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    /**
     * @brief Access underlying domain.
     */
    BanachSpace& domain();

    /**
     * @brief Access range space.
     */
    BanachSpace& range();

    /**
     * @brief Access underlying domain.
     */
    const BanachSpace& domain() const;

    /**
     * @brief Access range space
     */
    const BanachSpace& range() const;

    std::shared_ptr<BanachSpace> sharedDomain() const;

  private:
    std::shared_ptr<BanachSpace> domain_ = nullptr, range_ = nullptr;
  };
}
#endif // ALGORITHM_OPERATOR_HH
