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
    Functional(std::shared_ptr<Algorithm::AbstractFunctional> impl);

    double operator()(const FunctionSpaceElement& x);

    AbstractFunctional& impl();

    const AbstractFunctional& impl() const;

  protected:
    std::shared_ptr<AbstractFunctional> impl_ = nullptr;
  };

  template <class Implementation, class... Args>
  Functional makeFunctional(Args&&... args)
  {
    return Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_FUNCTIONAL_HH
