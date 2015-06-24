#ifndef ALGORITHM_C2_FUNCTIONAL_HH
#define ALGORITHM_C2_FUNCTIONAL_HH

#include "c1Functional.hh"

namespace Algorithm
{
  class AbstractC2Functional;
  class C1Operator;

  class C2Functional : public C1Functional
  {
  public:
    C2Functional(std::shared_ptr<AbstractC2Functional> impl);

    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    double d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    C1Operator getDerivative() const;

    AbstractC2Functional& impl();

    const AbstractC2Functional& impl() const;

    using C1Functional::operator ();
  private:
    std::shared_ptr<AbstractC2Functional> impl_;
  };

  template <class Implementation, class... Args>
  C2Functional makeC2Functional(Args&&... args)
  {
    return C2Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C2_FUNCTIONAL_HH
