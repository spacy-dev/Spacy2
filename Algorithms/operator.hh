#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <memory>
#include "Interface/Operator/abstractOperator.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class Operator
  {
  public:
    Operator(std::shared_ptr<AbstractOperator> impl);

    virtual FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    AbstractOperator& impl();

    const AbstractOperator& impl() const;

  protected:
    std::shared_ptr<AbstractOperator> impl_ = nullptr;
  };

  template <class Implementation, class... Args>
  Operator makeOperator(Args&&... args)
  {
    return Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_OPERATOR_HH
