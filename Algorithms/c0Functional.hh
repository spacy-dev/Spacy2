#ifndef ALGORITHM_C0_FUNCTIONAL_HH
#define ALGORITHM_C0_FUNCTIONAL_HH

#include <memory>

namespace Algorithm
{
  class AbstractC0Functional;
  class FunctionSpaceElement;

  class C0Functional
  {
  public:
    C0Functional(std::shared_ptr<AbstractC0Functional> impl);

//    void setArgument(const FunctionSpaceElement& x);

    double operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement operator()() const;

  protected:
    std::shared_ptr<AbstractC0Functional> impl_ = nullptr;
  };

  template <class Implementation, class... Args>
  C0Functional makeC0Functional(Args&&... args)
  {
    return C0Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_OPERATOR_HH
