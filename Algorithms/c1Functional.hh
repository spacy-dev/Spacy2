#ifndef ALGORITHM_C1_FUNCTIONAL_HH
#define ALGORITHM_C1_FUNCTIONAL_HH

#include "functional.hh"

namespace Algorithm
{
  class AbstractC1Functional;
  class FunctionSpaceElement;

  class C1Functional : public Functional
  {
  public:
    C1Functional(std::shared_ptr<AbstractC1Functional> impl);

//    void setArgument(const FunctionSpaceElement& x);

  //  double operator()(const FunctionSpaceElement& x) const;

    double d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    AbstractC1Functional& impl();

    const AbstractC1Functional& impl() const;

    //using Functional::operator();

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
