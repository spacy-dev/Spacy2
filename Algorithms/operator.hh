#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "functionSpace.hh"
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
    FunctionSpace& domain();

    /**
     * @brief Access range space.
     */
    FunctionSpace& range();

    /**
     * @brief Access underlying domain.
     */
    const FunctionSpace& domain() const;

    /**
     * @brief Access range space
     */
    const FunctionSpace& range() const;

    /**
     * @brief Access shared pointer to underlying domain.
     */
//    std::shared_ptr<FunctionSpace> sharedDomain() const;

  private:
    FunctionSpace domain_, range_;
//    std::shared_ptr<FunctionSpace> domain_ = nullptr, range_ = nullptr;
  };
}
#endif // ALGORITHM_OPERATOR_HH
