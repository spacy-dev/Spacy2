#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class Operator : public Mixin::UniqueImpl<Interface::AbstractOperator>
  {
  public:
    Operator(std::unique_ptr<Interface::AbstractOperator>&& impl);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;
  };
}
#endif // ALGORITHM_OPERATOR_HH
