#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>

#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class Operator : public Mixin::SharedImpl<AbstractOperator>
  {
  public:
    Operator(std::shared_ptr<AbstractOperator> impl);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;
  };

  template <class Implementation, class... Args>
  Operator makeOperator(Args&&... args)
  {
    return Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_OPERATOR_HH
