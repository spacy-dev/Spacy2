#ifndef ALGORITHM_FUNCTIONAL_HH
#define ALGORITHM_FUNCTIONAL_HH

#include <memory>

#include "Util/impl.hh"

namespace Algorithm
{
  class AbstractFunctional;
  class FunctionSpaceElement;

  class Functional : public SharedImpl<AbstractFunctional>
  {
  public:
    Functional(std::shared_ptr<Algorithm::AbstractFunctional> impl);

    double operator()(const FunctionSpaceElement& x);
  };

  template <class Implementation, class... Args>
  Functional makeFunctional(Args&&... args)
  {
    return Functional( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_FUNCTIONAL_HH
