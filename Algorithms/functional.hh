#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

namespace Algorithm
{
  class AbstractFunctional;
  class FunctionSpaceElement;

  class Functional
  {
  public:
    Functional(std::shared_ptr<AbstractFunctional> impl);

//    void setArgument(const FunctionSpaceElement& x);

    double operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement operator()() const;

  protected:
    std::shared_ptr<AbstractFunctional> impl_ = nullptr;
  };
}
#endif // ALGORITHM_OPERATOR_HH
