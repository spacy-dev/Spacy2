#ifndef ALGORITHM_DIFFERENTIABLEFUNCTIONAL_HH
#define ALGORITHM_DIFFERENTIABLEFUNCTIONAL_HH

#include "functional.hh"

namespace Algorithm
{
  class AbstractDifferentiableFunctional;
  class FunctionSpaceElement;

  class DifferentiableFunctional : public Functional
  {
  public:
    DifferentiableFunctional(std::shared_ptr<AbstractDifferentiableFunctional> impl);

//    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    double d1(const FunctionSpaceElement& dx) const;

    AbstractDifferentiableFunctional& impl();

    const AbstractDifferentiableFunctional& impl() const;

  private:
    std::shared_ptr<AbstractDifferentiableFunctional> impl_;
  };
}
#endif // ALGORITHM_DIFFERENTIABLEFUNCTIONAL_HH
