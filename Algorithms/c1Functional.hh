#ifndef ALGORITHM_C1_FUNCTIONAL_HH
#define ALGORITHM_C1_FUNCTIONAL_HH

#include "c0Functional.hh"

namespace Algorithm
{
  class AbstractC1Functional;
  class FunctionSpaceElement;

  class C1Functional : public C0Functional
  {
  public:
    C1Functional(std::shared_ptr<AbstractC1Functional> impl);

//    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    double d1(const FunctionSpaceElement& dx) const;

    AbstractC1Functional& impl();

    const AbstractC1Functional& impl() const;

  private:
    std::shared_ptr<AbstractC1Functional> impl_;
  };

  template <class Implementation, class... Args>
  C1Functional makeC1Functional(Args&&... args)
  {
    return C1Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C1_FUNCTIONAL_HH
